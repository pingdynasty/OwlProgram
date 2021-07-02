#ifndef __RampOscillator_h
#define __RampOscillator_h

#include "Oscillator.h"

/**
 * Ramp oscillator generates rising output values from -1 to 1.
 */
class RampOscillator : public OscillatorTemplate<RampOscillator> {
public:
  static constexpr float begin_phase = -1;
  static constexpr float end_phase = 1;
  float getSample(){
    return phase;
  }
};

class AntialisedRampOscillator : public OscillatorTemplate<AntialisedRampOscillator> {
public:
  static constexpr float begin_phase = -1;
  static constexpr float end_phase = 1;
  float getSample(){
    return phase - polyblep(phase, incr);
  }
};

class TriangleOscillator : public OscillatorTemplate<TriangleOscillator> {
public:
  static constexpr float begin_phase = -1;
  static constexpr float end_phase = 1;
  float getSample(){
    return -2 * (fabsf(phase) - 0.5);
  }
};

class SquareOscillator : public OscillatorTemplate<SquareOscillator> {
protected:
  float pw = 0.5;
public:
  static constexpr float begin_phase = 0;
  static constexpr float end_phase = 1;
  /**
   * Set pulse width to a value between 0 and 1
   */
  void setPulseWidth(float value){
    pw = value;
  }
  float getSample(){
    return phase < pw ? 1 : -1;
  }
};

class InvertedSquareOscillator : public OscillatorTemplate<InvertedSquareOscillator> {
protected:
  float pw = 0.5;
public:
  static constexpr float begin_phase = 0;
  static constexpr float end_phase = 1;
  /**
   * Set pulse width to a value between 0 and 1
   */
  void setPulseWidth(float value){
    pw = value;
  }
  float getSample(){
    return phase < pw ? -1 : 1;
  }
};

class AntialiasedSquareOscillator : public OscillatorTemplate<AntialiasedSquareOscillator> {
protected:
  float pw = 0.5f;
public:
  static constexpr float begin_phase = 0;
  static constexpr float end_phase = 1;
  /**
   * Set pulse width to a value between 0 and 1
   */
  void setPulseWidth(float value){
    pw = value;
  }
  float getSample(){
    float sample = phase < pw ? 1 : -1;
    sample += polyblep(incr, phase);
    sample -= polyblep(incr, fmod(phase + 1 + pw, 2));
    return sample;
  }
};

class AntialiasedTriangleOscillator : public OscillatorTemplate<AntialiasedTriangleOscillator> {
protected:
  float pw = 0.5f;
  float previousSample = 0;
public:
  static constexpr float begin_phase = 0;
  static constexpr float end_phase = 1;
  /**
   * Set pulse width to a value between 0 and 1
   */
  void setPulseWidth(float value){
    pw = value;
  }
  float getSample(){
    float sample = phase < pw ? 1 : -1;
    sample += polyblep(incr, phase);
    sample -= polyblep(incr, fmod(phase + 1 + pw, 2));
    // Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1] = A * (x[n] - y[n-1]) + y[n-1]
    float lambda = incr;
    // float lambda = incr*2*M_PI;
    sample = lambda * sample + (1 - lambda) * previousSample;
    // sample = lambda * (sample - previousSample) + previousSample;
    previousSample = sample;
    return sample;
  }
};

#endif /* __RampOscillator_h */
