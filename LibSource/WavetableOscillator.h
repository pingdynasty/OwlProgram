#ifndef __WavetableOscillator_h__
#define __WavetableOscillator_h__

#include "FloatArray.h"
#include "Oscillator.h"

class WavetableOscillator : public Oscillator {
private:
  float mul;
  FloatArray wave;
  float acc;
  float inc;
public:
  WavetableOscillator(float sr, const FloatArray wavetable);
  void setSampleRate(float sr);
  float getSampleRate();
  void setFrequency(float freq);
  float getFrequency();
  float getSample(float phase);
  void setPhase(float phase);
  void reset(){
    setPhase(0);
  }
  float getPhase();
  float generate();
  float generate(float fm);
  void generate(FloatArray samples);
  void generate(FloatArray output, FloatArray fm);
  static WavetableOscillator* create(float sr, size_t size);
  static void destroy(WavetableOscillator* osc);
};

#endif /* __WavetableOscillator_h__ */
