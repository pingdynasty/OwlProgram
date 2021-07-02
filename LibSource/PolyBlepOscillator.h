#ifndef __PolyBlepOscillator_h__
#define __PolyBlepOscillator_h__

#include "FloatArray.h"
#include "polyblep/dsp/oscillator.h"
#include "Oscillator.h"

class PolyBlepOscillator : public Oscillator {
private:
  float mul;
  stmlib::Oscillator osc;
  float nfreq, shape, pw;
public:
  PolyBlepOscillator(float sr=48000);
  PolyBlepOscillator(float freq, float sr);
  void setSampleRate(float sr);
  float getSampleRate();
  void setFrequency(float freq);
  float getFrequency();
  /** set waveform shape: 0.0 for saw, 1.0 for square wave */
  void setShape(float value);
  float getShape();
  /** set square wave pulse width: 0.0 to 1.0 for 0% to 100% */
  void setPulseWidth(float value);
  float getPulseWidth();
  void setPhase(float phase);
  float getPhase();
  void reset(){
    setPhase(0);
  }
  float generate();
  float generate(float fm);
  /* put a block of output samples into @param output */
  void generate(FloatArray output);
  /* put a block of output samples into @param output, frequency modulated by @param fm */
  void generate(FloatArray output, FloatArray fm);
  /* put a block of output samples into @param samples, 
     with frequency determined by samples in @param frequency */
  void getSamples(FloatArray output, FloatArray freq);
  static PolyBlepOscillator* create(float sr);
  static PolyBlepOscillator* create(float freq, float sr);
  static void destroy(PolyBlepOscillator* osc);
};

#endif /* __PolyBlepOscillator_h__ */
