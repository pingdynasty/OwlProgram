#ifndef __LINEAR_ADSR_ENVELOPE_H
#define __LINEAR_ADSR_ENVELOPE_H

#include "Envelope.h"

/**
 * Linear ADSR Envelope
 */
class LinearAdsrEnvelope : public Envelope {
protected:
  enum EnvelopeStage { kAttack, kDecay, kSustain, kRelease, kIdle };
  enum EnvelopeTrigger { kGate, kTrigger };
public:
  LinearAdsrEnvelope(float sampleRate);
  virtual ~LinearAdsrEnvelope();
  using Envelope::process;
  using Envelope::trigger;
  using SignalGenerator::generate;
  void setSampleRate(float sampleRate);
  void setAttack(float newAttack);
  void setDecay(float newDecay);
  void setRelease(float newRelease);
  void setSustain(float newSustain);
  void trigger(bool state, int triggerDelay);
  void setRetrigger(bool on);
  void gate(bool state);
  void gate(bool state, int gateDelay);
  float getLevel();
  void setLevel(float newLevel);
  /**
   * Produce the next envelope sample.
   */
  float generate();
  [[deprecated("use generate() instead.")]]
  float getNextSample(){
    return generate(); // increments envelope one step
  }  
  [[deprecated("use generate() instead.")]]
  void getEnvelope(FloatArray output){
    generate(output); // increments envelope by output buffer length
  }
  [[deprecated("use process() instead.")]]
  void attenuate(FloatArray buf){
    process(buf, buf); // increments envelope by buffer length
  }
  static LinearAdsrEnvelope* create(float sampleRate){
    return new LinearAdsrEnvelope(sampleRate);
  }
  static void destroy(LinearAdsrEnvelope* env){
    delete env;
  }
protected:
  float sampleRate;
  EnvelopeStage stage;
  EnvelopeTrigger trig;
  bool retrigger;
  float level;
  float attackIncrement;
  float decayIncrement;
  float releaseIncrement;
  float sustain;
  bool gateState;
  int gateTime;
};

#endif /* __LINEAR_ADSR_ENVELOPE_H */
