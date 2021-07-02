#ifndef __Oscillator_h
#define __Oscillator_h

#include "basicmaths.h"
#include "FloatArray.h"
#include "SignalGenerator.h"

/**
 * An Oscillator is a SignalGenerator that operates at a given frequency
 * and that can be frequency modulated.
 */
class Oscillator : public SignalGenerator {
public:
  Oscillator(){}
  virtual ~Oscillator(){}
  using SignalGenerator::generate;
  /**
   * Set oscillator sample rate
   */
  virtual void setSampleRate(float value){}
  /**
   * Set oscillator frequency in Hertz
   */
  virtual void setFrequency(float value) = 0;
  /**
   * Get oscillator frequency in Hertz
   */
  virtual float getFrequency() = 0;
  /**
   * Set current oscillator phase in radians
   * @param phase a value between 0 and 2*pi
   */
  virtual void setPhase(float phase) = 0;
  /**
   * Get current oscillator phase in radians
   * @return a value between 0 and 2*pi
   */
  virtual float getPhase() = 0;
  /**
   * Reset oscillator (typically resets phase)
   */
  virtual void reset() = 0;
  /**
   * Produce a sample with frequency modulation.
   */
  virtual float generate(float fm) = 0;
  /**
   * Produce a block of samples with frequency modulation.
   */
  virtual void generate(FloatArray output, FloatArray fm){
    for(size_t i=0; i<output.getSize(); ++i)
      output[i] = generate(fm[i]);
  }
  [[deprecated("use generate() instead.")]]
  float getNextSample(){ return generate(); }
  [[deprecated("use generate() instead.")]]
  float getNextSample(float fm){ return generate(fm); }
  [[deprecated("use generate() instead.")]]
  void getSamples(FloatArray output){
    generate(output);
  }
  [[deprecated("use generate() instead.")]]
  void getSamples(FloatArray output, FloatArray fm){
    generate(output, fm);
  }
};

template<class T>
class OscillatorTemplate : public Oscillator {
protected:
  float mul;
  float phase;
  float incr;
public:
  void setSampleRate(float sr){
    float freq = getFrequency();
    mul = (T::end_phase - T::begin_phase)/sr;
    setFrequency(freq);
  }
  float getSampleRate(){
    return (T::end_phase - T::begin_phase)/mul;
  }
  void setFrequency(float freq){
    incr = freq*mul;
  }
  float getFrequency(){
    return incr/mul;
  }
  void setPhase(float ph){
    phase = (T::end_phase - T::begin_phase)*ph/(2*M_PI) + T::begin_phase;
  }
  float getPhase(){
    // return phase 0 to 2*pi
    return (phase - T::begin_phase)*2*M_PI/(T::end_phase - T::begin_phase);
  }
  void reset(){
    phase = T::begin_phase;
  }
  float generate(){
    float sample = static_cast<T*>(this)->getSample();
    phase += incr;
    if(phase >= T::end_phase)
      phase -= (T::end_phase - T::begin_phase);
    return sample;
  }
  float generate(float fm){
    float sample = static_cast<T*>(this)->getSample();
    phase += incr * (1 + fm);
    if(phase >= T::end_phase)
      phase -= (T::end_phase - T::begin_phase);
    return sample;
  }  
  using Oscillator::generate;
  static T* create(float sr){
    T* obj = new T();
    obj->setSampleRate(sr);
    return obj;
  }
  static void destroy(T* osc){
    delete osc;
  }
protected:
  static float polyblep(float t, float dt){
    // PolyBLEP by various
    // http://research.spa.aalto.fi/publications/papers/smc2010-phaseshaping/
    // https://www.kvraudio.com/forum/viewtopic.php?t=375517
    // http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
    // https://www.metafunction.co.uk/post/all-about-digital-oscillators-part-2-blits-bleps
    // if t and dt are normalised before call then end/begin phase are not needed    
    if(t < T::begin_phase + dt){
      dt = dt / (T::end_phase - T::begin_phase); // normalise phase increment
      t = (t - T::begin_phase) / dt; // distance from discontinuity
      return t+t - t*t - 1;
    }else if(t > T::end_phase - dt){
      dt = dt / (T::end_phase - T::begin_phase); // normalise phase increment
      t = (t - T::end_phase) / dt; // distance from discontinuity
      return t*t + t+t + 1;
    }
    return 0;
  }
};

#endif /* __Oscillator_h */
