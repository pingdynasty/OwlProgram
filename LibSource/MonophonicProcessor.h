#ifndef __MonophonicProcessor_h__
#define __MonophonicProcessor_h__

#include "MidiProcessor.h"
#include "SignalGenerator.h"
#include "VoiceAllocator.h"

template<class SynthVoice>
class MonophonicProcessor : public VoiceAllocator<SynthVoice, 1> {
  typedef VoiceAllocator<SynthVoice, 1> Allocator;
protected:
  uint8_t notes[16];
  uint8_t lastNote = 0;
public:
  MonophonicProcessor() {}
  virtual void noteOn(MidiMessage msg){
    if(lastNote < 16)
      notes[lastNote++] = msg.getNote();
    Allocator::voice[0]->noteOn(msg);
  }
  virtual void noteOff(MidiMessage msg){
    uint8_t note = msg.getNote();
    int i;
    for(i = 0; i < lastNote; ++i) {
      if(notes[i] == note)
	break;
    }
    if(lastNote > 1) {
      lastNote--;
      while (i < lastNote) {
	notes[i] = notes[i + 1];
	i++;
      }
      Allocator::voice[0]->setNote(notes[lastNote - 1]);
    }else{
      Allocator::voice[0]->gate(false);
      lastNote = 0;
    }
  }
  void allNotesOff() {
    Allocator::voice[0]->gate(false);
    lastNote = 0;
  }
  void controlChange(MidiMessage msg){
    Allocator::voice[0]->controlChange(msg);
  }
  void pitchbend(MidiMessage msg){
    Allocator::voice[0]->pitchbend(msg);
  }
};

#endif // __MonophonicProcessor_h__
