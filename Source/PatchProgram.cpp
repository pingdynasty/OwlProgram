#include <string> /* include this here to avoid errors with std::min/std::max later */
#include "ProgramVector.h"
#include "ServiceCall.h"
#include "SampleBuffer.hpp"
#include "PatchProcessor.h"
#include "message.h"
#include "Patch.h"
#include "main.h"
#include "heap.h"
#include "system_tables.h"
#ifdef USE_MIDI_CALLBACK
#include "MidiMessage.h"
#endif
#include "registerpatch.h"

static PatchProcessor processor;
PatchProcessor* getInitialisingPatchProcessor(){
  return &processor;
}

void doSetPatchParameter(uint8_t id, int16_t value){
  ProgramVector* vec = getProgramVector();
  if(vec->checksum >= PROGRAM_VECTOR_CHECKSUM_V12 &&
     vec->setPatchParameter != NULL && vec->parameters[id] != value)
    vec->setPatchParameter(id, value);
}

void doSetButton(uint8_t id, uint16_t value, uint16_t samples){
  ProgramVector* vec = getProgramVector();
  if(vec->checksum >= PROGRAM_VECTOR_CHECKSUM_V12 &&
     // // if it is not a MIDI note, check that value has changed
     // (id > 15 || (bool)(vec->buttons&(1<<id)) != (bool)value) &&
     vec->setButton != NULL){
    vec->setButton((PatchButtonId)id, value, samples);
    if(id < 16){
      if(value)
	vec->buttons |= (1<<id);
      else
	vec->buttons &= ~(1<<id);
    }
  }
}

void onButtonChanged(uint8_t id, uint16_t value, uint16_t samples){
  if(processor.patch != NULL)
    processor.patch->buttonChanged((PatchButtonId)id, value, samples);
}

#ifdef USE_MIDI_CALLBACK
void onMidiCallback(uint8_t port, uint8_t status, uint8_t d1, uint8_t d2){
  static MidiMessage msg;
  if(processor.patch != NULL){
    msg.data[0] = port;
    msg.data[1] = status;
    msg.data[2] = d1;
    msg.data[3] = d2;
    processor.patch->processMidi(msg);
  }
}

static void (*midi_send_callback)(uint8_t, uint8_t, uint8_t, uint8_t) = NULL;
void doMidiSend(uint8_t port, uint8_t d0, uint8_t d1, uint8_t d2){
  if(midi_send_callback != NULL)
    midi_send_callback(port, d0, d1, d2);
}
#endif /* USE_MIDI_CALLBACK */

#define REGISTER_PATCH(T, STR, IN, OUT) do { registerPatch(STR, IN, OUT); setPatch(STR, new T); }while(0)

void registerPatch(const char* name, uint8_t inputs, uint8_t outputs){
  if(getProgramVector()->registerPatch != NULL)
    getProgramVector()->registerPatch(name, inputs, outputs);
}

void setPatch(const char* name, Patch* patch){
  if(patch == NULL)
    error(OUT_OF_MEMORY_ERROR_STATUS, "Out of memory");
  processor.setPatch(patch, name);
}

static SampleBuffer* samples = NULL;
void setup(ProgramVector* pv){
  setSystemTables(pv);
#ifdef USE_MIDI_CALLBACK
  void* midiRxArgs[] = {(void*)SYSTEM_FUNCTION_MIDI, (void*)&onMidiCallback};
  getProgramVector()->serviceCall(OWL_SERVICE_REGISTER_CALLBACK, midiRxArgs, 2);

  midi_send_callback = NULL;
  void* midiTxArgs[] = {(void*)SYSTEM_FUNCTION_MIDI, &midi_send_callback};
  getProgramVector()->serviceCall(OWL_SERVICE_REQUEST_CALLBACK, midiTxArgs, 2);

#endif /* USE_MIDI_CALLBACK */
  if(samples != NULL)
    SampleBuffer::destroy(samples);
#include "registerpatch.cpp"
  samples = SampleBuffer::create(pv->audio_format, pv->audio_blocksize);
  if(samples == NULL)
    error(CONFIGURATION_ERROR_STATUS, "Unsupported audio format");
}

void run(ProgramVector* pv){
  for(;;){
    pv->programReady();
    samples->split(pv->audio_input);
    processor.setParameterValues(pv->parameters);
    processor.patch->processAudio(*samples);
    samples->comb(pv->audio_output);
  }
}
