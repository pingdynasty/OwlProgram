#ifndef __AgnesiOscillator_h
#define __AgnesiOscillator_h

#include "Oscillator.h"

/**
 * Oscillator that produces an Agnesi curve (Witch of Agnesi)
 * With a=0.5, the output is between near 0 (0.038 for N=5) and 1.
 * N sets the x range for one half period, which determines the y offset.
 */
class AgnesiOscillator : public Oscillator {
private:
  const float sr;
  float a;
  float N;
  float incr = 0;
  float x = 0;
  float offset = 0;
  float gain = 1;
public:
  AgnesiOscillator(float sr, float a, float N)
    : sr(sr), a(a), N(N) {}
  static float agnesi(float x, float a){
    return (8*a*a*a) / (x*x + 4*a*a);
  }
  void setFrequency(float freq){
    incr = 2*N*freq/sr;
  }
  float getFrequency(){
    return incr*sr/(2*N);
  }
  void setPhase(float phase){
    x = N*(phase-M_PI)/M_PI;
  }
  float getPhase(){
    return M_PI*x/N+M_PI;
  }
  void reset(){
    x = -N;
  }
  /**
   * Normalise offset and gain so that signal is between 0 and 1
   */
  void normalise(){
    offset = agnesi(N, a);
    gain = 2/(agnesi(0, a) - offset);
    offset += 0.5;
  }
  float generate(){
    float y = agnesi(x, a);
    x += incr;
    if(x > N)
      x -= 2*N;
    return gain*(y-offset);
  }
  float generate(float fm){
    // modulate coefficient 'a' instead of rate
    float y = agnesi(x, a*(1+fm));
    x += incr;
    if(x > N)
      x -= 2*N;
    return gain*(y-offset);
  }
  using SignalGenerator::generate;
  static AgnesiOscillator* create(float sr, float a=0.5, float N=5){
    AgnesiOscillator* osc = new AgnesiOscillator(sr, a, N);
    osc->normalise();
    return osc;
  }
  static void destroy(AgnesiOscillator* obj){
    delete obj;
  }
};

#endif /* __AgnesiOscillator_h */
