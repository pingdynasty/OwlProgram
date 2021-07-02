#ifndef SINE_OSCILLATOR_H
#define SINE_OSCILLATOR_H

#include "Oscillator.h"

class SineOscillator : public OscillatorTemplate<SineOscillator> {
public:
  static constexpr float begin_phase = 0;
  static constexpr float end_phase = 2*M_PI;
  float getSample(){
    return sinf(phase);
  }
  void generate(FloatArray output){
    size_t len = output.getSize();
    for(size_t i=0; i<len; ++i){
      output[i] = sinf(phase);
      phase += incr; // allow phase to overrun
    }
    phase = fmodf(phase, end_phase);
  }
  void generate(FloatArray output, FloatArray fm){
    size_t len = output.getSize();
    for(size_t i=0; i<len; ++i){
      output[i] = sinf(phase);
      phase += incr * (1 + fm[i]);
      // allow phase to overrun
    }
    phase = fmodf(phase, end_phase);
  }
  using OscillatorTemplate<SineOscillator>::generate;  
};

#endif /* SINE_OSCILLATOR_H */
