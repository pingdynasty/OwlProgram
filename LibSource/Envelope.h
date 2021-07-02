#ifndef ENVELOPE_HPP
#define ENVELOPE_HPP

#include "FloatArray.h"
#include "SignalGenerator.h"
#include "SignalProcessor.h"

class Envelope : public SignalGenerator, SignalProcessor {
public:
  using SignalGenerator::generate;
  virtual void trigger(){
    trigger(true, 0);
  }
  virtual void trigger(bool state){
    trigger(state, 0);
  }
  virtual void trigger(bool state, int triggerDelay) = 0;
  virtual void gate(bool state){
    gate(state, 0);
  }
  virtual void gate(bool state, int gateDelay){}
  virtual float process(float input){
    return input*generate();
  }
  /**
   * Attenuate samples in @param input by envelope and place results in @param output
   */
  virtual void process(FloatArray input, FloatArray output){
    for(size_t n = 0; n < output.getSize(); n++)
      output[n] = input[n]*generate();
  }
};

#endif /* ENVELOPE_HPP */
