#ifndef __SampleOscillator_h
#define __SampleOscillator_h

#include "Oscillator.h"

template<InterpolationMethod im>
class SampleOscillator : public Oscillator {
public:
  enum RepeatMode {
	REPEAT_REVERSE = -1,
	REPEAT_NONE    = 0,
	REPEAT_FORWARD = 1
  };
private:
  const float sr;
  FloatArray buffer;
  float pos = 0.0f;
  size_t size;
  float rate = 1.0f;
  RepeatMode repeat_mode = REPEAT_NONE;
  static constexpr float MIDDLE_C = 261.6;
public:

 SampleOscillator(float sr, FloatArray sample)
    : sr(sr)
    , buffer(sample) {
    size = buffer.getSize() - 1;
  }
  void setFrequency(float freq) {
    rate = copysignf(freq / MIDDLE_C, rate); // preserve sign
  }
  float getFrequency(){
    return rate*MIDDLE_C;
  }
  void setSample(FloatArray sample){
    buffer = sample;
    size = buffer.getSize() -1;
    pos = 0;
  }
  FloatArray getSample(){
    return buffer;
  }
  void setRepeatMode(RepeatMode mode){
    repeat_mode = mode;
  }
  void reset() {
    pos = 0;
  }
  float interpolate(float index, FloatArray data);
  void setPhase(float phase){
    pos = size*phase/(2*M_PI);
  }
  float getPhase(){
    return pos*2*M_PI/size;
  }
  float generate(float fm) {
    return generate();
  }
  float generate() {
    if(pos < 0){
      if(repeat_mode == REPEAT_REVERSE){
	rate *= -1;
	pos = 0;
      }else{
	return 0;
      }
    }else if(pos > size){
      if(repeat_mode == REPEAT_FORWARD){
	pos = 0;
      }else if(repeat_mode == REPEAT_REVERSE){
	rate *= -1;
	pos = size;
      }else{
	return 0;
      }
    }
    float sample = interpolate(pos, buffer);
    pos += rate;
    return sample;
  }
  void generate(FloatArray output){
    size_t len = output.getSize();
    float* dest = output;
    if(rate > 0){
      while(pos + rate*len > size){
	float remain = size - pos;
	size_t steps = (size_t)(remain/rate);
	for(size_t i=0; i<steps; ++i){
	  *dest++ = interpolate(pos, buffer);
	  pos += rate;
	}
	len -= steps;
	if(repeat_mode == REPEAT_FORWARD){
	  pos -= size;
	}else if(repeat_mode == REPEAT_REVERSE){
	  rate *= -1;
	  pos = size;
	}else{
	  return;
	}
      }
    }else{
      while(pos + rate*len < 0){
	float remain = pos;
	size_t steps = (size_t)(remain/rate);
	for(size_t i=0; i<steps; ++i){
	  *dest++ = interpolate(pos, buffer);
	  pos += rate;
	}
	len -= steps;
	if(repeat_mode == REPEAT_REVERSE){
	  rate *= -1;
	  pos = 0;
	}else{
	  return;
	}
      }
    }
    for(size_t i=0; i<len; ++i){
      *dest++ = interpolate(pos, buffer);
      pos += rate;
    }
  }
  void generate(FloatArray output, FloatArray fm){
    return generate(output);
  }
  size_t findZeroCrossing(size_t index) {
    size_t len = buffer.getSize()-1;
    size_t i = min(index, len);
    if (buffer[i] > 0)
      while (i < len && buffer[i] > 0)
	i++;
    else
      while (i < len && buffer[i] < 0)
	i++;
    return i;
  }
  void setDuration(float duration) {
    size_t samples = duration * getSampleLength()+sr*0.025; // min 25 mS
    size = findZeroCrossing(samples);
  }
  /* @return the audio length in samples */
  size_t getSampleLength() {
    return buffer.getSize();
  }
  static SampleOscillator* create(float sr, FloatArray buf) {
    return new SampleOscillator(sr, buf);
  }
  static void destroy(SampleOscillator* obj) {
    delete obj;
  }
};

template<>
float SampleOscillator<LINEAR_INTERPOLATION>::interpolate(float index, FloatArray data) {
  size_t idx = (int)index;
  float low = data[idx];
  float high = data[idx + 1];
  float frac = index - idx;
  return Interpolator::linear(low, high, frac);
}

template<>
float SampleOscillator<COSINE_INTERPOLATION>::interpolate(float index, FloatArray data) {
  size_t idx = (int)index;
  return Interpolator::cosine(data[idx], data[idx + 1], index - idx);
}

typedef SampleOscillator<LINEAR_INTERPOLATION> LinearSampleOscillator;
typedef SampleOscillator<COSINE_INTERPOLATION> CosineSampleOscillator;

#endif /* __SampleOscillator_h */
