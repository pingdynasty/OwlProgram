#include "ServiceCall.h"
#include "FastPowTable.h"
#include "FastLogTable.h"
#include "basicmaths.h"
#include "fileio.h"

static float parameter_values[40] = {};
static uint32_t button_values = 0;
int errorcode = 0;

extern "C"{
  int serviceCall(int service, void** params, int len){
    int ret = -1;
    switch(service){
    case OWL_SERVICE_LOAD_RESOURCE:
      if(len == 4){
	const char* name = (const char*)params[0];
	uint8_t** buffer = (uint8_t**)params[1];
	uint32_t offset = *(uint32_t*)params[2];
	uint32_t* max_size = (uint32_t*)params[3];
	printf("Service call (LOAD RESOURCE) : %s [%u:%u]\n", name, offset, *max_size);
	*max_size = fileread(name, buffer, *max_size);
	ret = OWL_SERVICE_OK;
      }else{
	printf("Service call (LOAD RESOURCE) Invalid");
      }	
      break;
    default:
      printf("Service call (todo) : %d\n", service);
      break;
    }
    return ret;
  }
  void error(int8_t code, const char* reason){
    printf("%s\n", reason);
    errorcode = -1;
    exit(errorcode);
  }
}

class StereoSampleBuffer : public AudioBuffer {
protected:
  FloatArray left;
  FloatArray right;
  uint16_t size;
  const float mul = 1/2147483648.0f;
public:
  StereoSampleBuffer(int blocksize){
    left = FloatArray::create(blocksize);
    right = FloatArray::create(blocksize);
    size = blocksize;
  }
  ~StereoSampleBuffer(){
    FloatArray::destroy(left);
    FloatArray::destroy(right);
  }
  void split16(int16_t* input, uint16_t blocksize){
    ASSERT(blocksize == size, "Invalid buffer size");
    for(int i=0; i<blocksize; ++i){
      left[i] = float(*input++)/INT16_MAX;
      right[i] = float(*input++)/INT16_MAX;
    }
  }
  void comb16(int16_t* output){
    int16_t* dest = output;
    for(int i=0; i<size; ++i){
      *dest++ = ((int16_t)(left[i] * INT16_MAX));
      *dest++ = ((int16_t)(right[i] * INT16_MAX));
    }
  }
  void clear(){
    left.clear();
    right.clear();
  }
  inline FloatArray getSamples(int channel){
    return channel == LEFT_CHANNEL ? left : right;
    // return channel == 0 ? FloatArray(left, size) : FloatArray(right, size);
  }
  inline int getChannels(){
    return 2;
  }
  inline int getSize(){
    return size;
  }
};

#ifdef ARM_MATH_CM0
extern "C" {
  // http://www.keil.com/forum/60479/
  void arm_bitreversal_32(uint32_t *pSrc, const uint16_t bitRevLen, const uint16_t *pBitRevTab){
  uint32_t r3 = (bitRevLen + 1) / 2;
  uint32_t *r2, *r6;
  uint32_t r4, r5;
  while(r3--)
  {
    r2 = (uint32_t *)((uint32_t)pSrc + pBitRevTab[0]);
    r6 = (uint32_t *)((uint32_t)pSrc + pBitRevTab[1]);
    r5 = r2[0];
    r4 = r6[0];
    r6[0] = r5;
    r2[0] = r4;
    r5 = r2[1];
    r4 = r6[1];
    r6[1] = r5;
    r2[1] = r4;
    pBitRevTab += 2;
  }
}
void arm_bitreversal_16(uint32_t *pSrc, const uint16_t bitRevLen, const uint16_t *pBitRevTab)
{
  ASSERT(false, "TODO: arm_bitreversal_16");
}
}
#endif

extern "C"{
  void doSetButton(uint8_t bid, uint16_t value, uint16_t samples){
    printf("Set button %c: %d\n", 'A'+bid-4, value);
    if(value)
      button_values |= (1<<bid);
    else
      button_values &= ~(1<<bid);
  }
  void doSetPatchParameter(uint8_t pid, int16_t value){
    printf("Set parameter %c: %d\n", 'A'+pid, value);
    if(pid < 40)
      parameter_values[pid] = value/4096.0f;
  }

  void assert_failed(const char* msg, const char* location, int line){
    printf("Assertion failed: %s, in %s line %d\n", msg, location, line);
    exit(-1);
  }
}

const char hexnumerals[] = "0123456789abcdef";
char* msg_itoa(int val, int base){
  static char buf[13] = {0};
  int i = 11;
  unsigned int part = abs(val);
  do{
    buf[i--] = hexnumerals[part % base];
    part /= base;
  }while(part && i);
  if(val < 0)
    buf[i--] = '-';
  return &buf[i+1];
}

char* msg_ftoa(float val, int base){
  static char buf[16] = {0};
  int i = 14;
  // print 2 decimal points
  unsigned int part = abs((int)((val-int(val))*100));
  do{
    buf[i--] = hexnumerals[part % base];
    part /= base;
  }while(i > 12);
  buf[i--] = '.';
  part = abs(int(val));
  do{
    buf[i--] = hexnumerals[part % base];
    part /= base;
  }while(part && i);
  if(val < 0.0f)
    buf[i--] = '-';
  return &buf[i+1];
}

void debugMessage(char const* msg, int a){
  printf("%s %d\n", msg, a);
}

void debugMessage(char const* msg, float a){
  printf("%s %f\n", msg, a);
}

void debugMessage(char const* msg, int a, int b){
  printf("%s %d %d\n", msg, a, b);
}

void debugMessage(char const* msg, float a, float b){
  printf("%s %f %f\n", msg, a, b);
}

void debugMessage(char const* msg, int a, int b, int c){
  printf("%s %d %d %d\n", msg, a, b, c);
}

void debugMessage(char const* msg, float a, float b, float c){
  printf("%s %f %f %f\n", msg, a, b, c);
}

void debugMessage(char const* msg){
  printf("%s\n", msg);
}

AudioBuffer* AudioBuffer::create(int channels, int samples){
  return new ManagedMemoryBuffer(channels, samples);
}
AudioBuffer::~AudioBuffer(){}
void AudioBuffer::destroy(AudioBuffer* buffer){
  delete buffer;
}

Patch::Patch(){}
Patch::~Patch(){}
PatchProcessor::PatchProcessor(){
  fast_log_set_table(fast_log_table, fast_log_table_size);
  fast_pow_set_table(fast_pow_table, fast_pow_table_size);
}
void PatchProcessor::setPatch(Patch* patch, const char* name){
  if(patch == NULL)
    error(OUT_OF_MEMORY_ERROR_STATUS, "Out of memory");
  this->patch = patch;
}
PatchProcessor::~PatchProcessor(){}

void PatchProcessor::setPatchParameter(int pid, FloatParameter* param){
  printf("Set parameter %c: %f\n", 'A'+pid, param->getValue());
  if(pid < 40)
    parameter_values[pid] = param->getValue();
}

void PatchProcessor::setPatchParameter(int pid, IntParameter* param){
  printf("Set parameter %c: %d\n", 'A'+pid, param->getValue());
  if(pid < 40)
    parameter_values[pid] = param->getValue()/4096.0f;
}

Resource* Patch::getResource(char const* name){
  printf("Get resource %s\n", name);
  Resource* resource = Resource::load(name);
  if(resource == NULL)
    error(CONFIGURATION_ERROR_STATUS, "Missing Resource");
  return resource;
}

void Patch::registerParameter(PatchParameterId pid, const char* name){
  printf("Register parameter %c: %s\n", 'A'+pid, name);
}  

float Patch::getElapsedBlockTime(){ return 0; }
int Patch::getElapsedCycles(){ return 0; }

void Patch::processMidi(MidiMessage msg){}

void Patch::sendMidi(MidiMessage msg){
  printf("Sending MIDI [%x:%x:%x:%x]\n", msg.data[0], msg.data[1], msg.data[2], msg.data[3]);
}  

float Patch::getSampleRate(){
  return SAMPLE_RATE;
}

int Patch::getBlockSize(){
  return BLOCKSIZE;
}

float Patch::getBlockRate(){
  return SAMPLE_RATE/BLOCKSIZE;
}

int Patch::getNumberOfChannels(){
  return CHANNELS;
}

float Patch::getParameterValue(PatchParameterId pid){
  if(pid < 40)
    return parameter_values[pid];
  return 0.0f;
}

void Patch::setParameterValue(PatchParameterId pid, float value){
  printf("Set parameter %c: %f\n", 'A'+pid, value);
  if(pid < 40)
    parameter_values[pid] = value;
}

void Patch::setButton(PatchButtonId bid, uint16_t value, uint16_t samples){
  doSetButton(bid, value, samples);
}

bool Patch::isButtonPressed(PatchButtonId bid){
  return button_values & (1<<bid);
}
