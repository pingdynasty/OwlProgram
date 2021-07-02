#ifndef __VosimOscillator_h
#define __VosimOscillator_h

#include "Oscillator.h"

/**
 * VOSIM-type oscillator based on Rob Hordijk's Nord Modular patch.
 * ref: https://electro-music.com/nm_classic/015_workshops/Clavia/NordModularWorkshops&Threads/WerkMap/WorkShops/Hordijk1999-2000/VOSIM.html
*/
class VosimOscillator : public Oscillator {
private:
  const float fs;
  SineOscillator* formant1;
  SineOscillator* formant2;
  float phase = 0;
  float incr = 0;
public:
  VosimOscillator(float samplerate, SineOscillator* osc2, SineOscillator* osc1) :
    fs(samplerate), formant1(osc1) , formant2(osc2) {}
  float getFrequency(){
    return incr*fs;
  }
  void setFrequency(float freq){
    incr = freq/fs;
  }
  void reset(){
    phase = 0.0f;
  }
  void setPhase(float ph){
    phase = 1-ph/(2*M_PI);
  }
  float getPhase(){
    return (1-phase)*2*M_PI;
  }
  float getFormant1(){
    return formant1->getFrequency();
  }
  float getFormant2(){
    return formant1->getFrequency();
  }
  void setFormant1(float frequency){
    formant1->setFrequency(frequency);
  }
  void setFormant2(float frequency){
    formant2->setFrequency(frequency);
  }
  using Oscillator::generate;
  float generate(float fm){
    float s1 = formant1->generate();
    float s2 = formant2->generate();
    float sample = (s1*s1 + s2*s2)*phase - 1;
    float dt = incr * (1 + fm);
    // sample = 2*phase-1;
    // sample -= polyblep(phase, dt);
    // polyblep works on sawtooth but doesn't help vosim
    phase -= dt;
    if(phase <= 0.0){
      phase += 1.0;
#if 0
      float ph = getPhase();
      formant1->setPhase(ph);
      formant2->setPhase(ph);
#else
      formant1->reset();
      formant2->reset();
#endif
    }
    return sample;
  }
  float generate(){
    float s1 = formant1->generate();
    float s2 = formant2->generate();
    float sample = (s1*s1 + s2*s2)*phase - 1;
    // sample -= polyblep(1-phase, incr);
    phase -= incr;
    if(phase <= 0.0){
      phase += 1.0;
#if 0
      float ph = getPhase();
      formant1->setPhase(ph);
      formant2->setPhase(ph);
#else
      formant1->reset();
      formant2->reset();
#endif
    }
    return sample;
  }
  static VosimOscillator* create(float sr){
    SineOscillator* f1 = SineOscillator::create(sr);
    SineOscillator* f2 = SineOscillator::create(sr);
    return new VosimOscillator(sr, f1, f2);
  }
  static void destroy(VosimOscillator* obj){
    SineOscillator::destroy(obj->formant1);
    SineOscillator::destroy(obj->formant2);
    delete obj;
  }
};

#endif  /* __VosimOscillator_h */
