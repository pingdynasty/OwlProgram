#ifndef __SignalGenerator_h__
#define __SignalGenerator_h__

#include "FloatArray.h"

/**
 * Base class for signal generators such as Oscillators.
 * A SignalGenerator produces samples from -1 to 1 unless
 * otherwise stated.
 */
class SignalGenerator {
public:
  virtual ~SignalGenerator(){}
  /**
   * Produce the next consecutive sample.
   */
  virtual float generate() = 0;
  /**
   * Produce a block of samples
   */
  virtual void generate(FloatArray output){
    for(size_t i=0; i<output.getSize(); ++i)
      output[i] = generate();
  }
};

#endif // __SignalGenerator_h__