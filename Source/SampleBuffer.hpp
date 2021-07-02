#ifndef __SAMPLEBUFFER_H__
#define __SAMPLEBUFFER_H__

#include <stdint.h>
#include "Patch.h"
#include "device.h"
#ifdef ARM_CORTEX
#include "arm_math.h"
#define AUDIO_SATURATE_SAMPLES
#else
#undef AUDIO_SATURATE_SAMPLES
#endif //ARM_CORTEX

#define MULTIPLIER_31B 0x80000000
#define MULTIPLIER_30B 0x40000000
#define MULTIPLIER_23B 0x00800000

class SampleBuffer : public AudioBuffer {
protected:
  const size_t channels;
  const size_t blocksize;
  FloatArray* buffers;
public:
  SampleBuffer(size_t channels, size_t blocksize)
    :channels(channels), blocksize(blocksize) {
    buffers = new FloatArray[channels];
    for(size_t i=0; i<channels; ++i)
      buffers[i] = FloatArray::create(blocksize);
  }
  ~SampleBuffer(){
    for(size_t i=0; i<channels; ++i)
      FloatArray::destroy(buffers[i]);
    delete[] buffers;
  }
  virtual void split(int32_t* input) = 0;
  virtual void comb(int32_t* output) = 0;
  void clear(){
    for(size_t i=0; i<channels; ++i)
      buffers[i].clear();
  }
  inline FloatArray getSamples(int channel){
    if(channel < channels)
      return buffers[channel];
    return FloatArray();
  }
  inline int getChannels(){
    return channels;
  }
  inline int getSize(){
    return blocksize;
  }
  static SampleBuffer* create(uint8_t format, size_t blocksize);
  static void destroy(SampleBuffer* obj){
    delete obj;
  }
};

class SampleBuffer32 : public SampleBuffer {
public:
  SampleBuffer32(size_t channels, size_t blocksize) : SampleBuffer(channels, blocksize){}
  void split(int32_t* input){
    const float mul = 1.0f/MULTIPLIER_31B;
    for(size_t j=0; j<channels; ++j){
      float* dst = buffers[j];
      int32_t* src = input+j;
      size_t len = blocksize;
      while(len--){
	*dst++ = (*src << 8) * mul;
	src += channels;
      }
    }
  }
  void comb(int32_t* output){
    const float mul = MULTIPLIER_23B;
    for(size_t j=0; j<channels; ++j){
      float* src = buffers[j];
      int32_t* dst = output+j;
      size_t len = blocksize;
      while(len--){
#ifdef AUDIO_SATURATE_SAMPLES
	*dst = __SSAT((q31_t)(*src++ * mul), 24);
#else
	*dst = ((int32_t)(*src++ * mul));
#endif
	dst += channels;
      }
    }
  }
};
  
class SampleBuffer24 : public SampleBuffer {
public:
  SampleBuffer24(size_t channels, size_t blocksize) : SampleBuffer(channels, blocksize){}
  void split(int32_t* data){
    const float mul = 1.0f/MULTIPLIER_31B;
    uint8_t* input = (uint8_t*)data;
    int32_t qint;
    for(size_t i=0; i<blocksize; ++i){
      qint =( *input++)<<16;
      qint |= (*input++)<<24;
      qint |= (*input++);
      qint |= (*input++)<<8;
      buffers[0][i] = qint * mul;
      qint =( *input++)<<16;
      qint |= (*input++)<<24;
      qint |= (*input++);
      qint |= (*input++)<<8;
      buffers[1][i] = qint * mul;
    }
  }
  void comb(int32_t* output){
    uint8_t* dest = (uint8_t*)output;
    int32_t qint;
    for(size_t i=0; i<blocksize; ++i){
#ifdef AUDIO_SATURATE_SAMPLES
      qint = __SSAT((q31_t)(buffers[0][i] * MULTIPLIER_23B), 24);
#else
      qint = buffers[0][i] * MULTIPLIER_23B;
#endif
      *dest++ = qint >> 24;
      *dest++ = qint >> 16;
      *dest++ = qint >> 8;
      *dest++ = qint;
#ifdef AUDIO_SATURATE_SAMPLES
      qint = __SSAT((q31_t)(buffers[1][i] * MULTIPLIER_23B), 24);
#else
      qint = buffers[1][i] * MULTIPLIER_23B;
#endif
      *dest++ = qint >> 24;
      *dest++ = qint >> 16;
      *dest++ = qint >> 8;
      *dest++ = qint;
    }
  }
};

class SampleBuffer16 : public SampleBuffer {
public:
  SampleBuffer16(size_t channels, size_t blocksize) : SampleBuffer(channels, blocksize){}
  void split(int32_t* data){
    const float mul = 1.0f/MULTIPLIER_31B;
    uint16_t* input = (uint16_t*)data;
    int32_t qint;
    for(size_t i=0; i<blocksize; ++i){
      qint = (*input++)<<16;
      qint |= *input++;
      buffers[0][i] = qint * mul;
      qint = (*input++)<<16;
      qint |= *input++;
      buffers[1][i] = qint * mul;
    }
  }
  void comb(int32_t* output){
    float* l = (float*)buffers[0];
    float* r = (float*)buffers[1];
    uint32_t blkCnt = blocksize;
    uint16_t* dst = (uint16_t*)output;
    int32_t qint;
    while(blkCnt > 0u){
      qint = *l++ * MULTIPLIER_31B;
      *dst++ = qint >> 16;
      *dst++ = qint & 0xffff;
      qint = *r++ * MULTIPLIER_31B;
      *dst++ = qint >> 16;
      *dst++ = qint & 0xffff;
      blkCnt--;
    }
  }
};

SampleBuffer* SampleBuffer::create(uint8_t format, size_t blocksize){
  size_t channels = format & AUDIO_FORMAT_CHANNEL_MASK;
  if(channels == 0)
    channels = 2;
  switch(format & AUDIO_FORMAT_FORMAT_MASK){
  case AUDIO_FORMAT_24B16:
    return new SampleBuffer16(channels, blocksize);
  case AUDIO_FORMAT_24B32:
    return new SampleBuffer32(channels, blocksize);
  default:
    return NULL; // unrecognised audio format
  }
}

#endif // __SAMPLEBUFFER_H__
