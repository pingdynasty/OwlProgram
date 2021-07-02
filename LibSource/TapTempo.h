#ifndef __TapTempo_h__
#define __TapTempo_h__

class TapTempo {
protected:
  const float sr;
  const size_t limit;
  size_t samples;
  size_t trig;
  bool ison;
public:
  TapTempo(float sr, size_t limit) : 
    sr(sr), limit(limit), samples(limit/2), trig(limit), ison(false) {
  }
  bool isOn(){
    return ison;
  }
  void trigger(bool on){
    trigger(on, 0);
  }
  void trigger(bool on, int delay){
    if(on && !ison){
      if(trig < limit){
	samples = trig + delay;
      }
      trig = 0;
    }
    ison = on;
  }
  void clock(){
    if(trig < limit)
      trig++;
  }
  void clock(size_t steps){
    trig += steps;
    if(trig > limit)
      trig = limit;
  }
  /**
   * Get tap tempo in BPM.
   */
  float getBeatsPerMinute(){
    return samplePeriodToBpm(samples, sr);
  }
  /**
   * Set tap tempo in BPM.
   */
  void setBeatsPerMinute(float bpm){
    samples = bpmToSamplePeriod(bpm, sr);
  }
  /**
   * Get tap tempo frequency.
   */
  float getFrequency(){
    return sr/samples;
  }
  /**
   * Set tap tempo frequency.
   */
  void setFrequency(float freq){
    samples = sr/freq;
  }
  /**
   * Get tap tempo period in samples.
   */
  size_t getPeriodInSamples(){
    return samples;
  }
  /**
   * Set tap tempo period in samples.
   */
  void setPeriodInSamples(size_t value){
    samples = value;
  }
  static float bpmToFrequency(float bpm){
    return bpm/60;
  }
  static float frequencyToBpm(float freq){
    return freq*60;
  }
  static float bpmToSamplePeriod(float bpm, float sr){
    return sr/bpmToFrequency(bpm);
  }
  static float samplePeriodToBpm(float samples, float sr){
    return frequencyToBpm(sr/samples);
  }
  static TapTempo* create(float sr, size_t limit){
    return new TapTempo(sr, limit);
  }
  static void destroy(TapTempo* obj){
    delete obj;
  }    
};

class AdjustableTapTempo : public TapTempo {
protected:
  uint16_t speed;
public:
  AdjustableTapTempo(float sr, size_t limit) :
    TapTempo(sr, limit), speed(2048) {}
  void resetAdjustment(uint16_t s){
    speed = s;
  }
  /**
   * Adjust the tap tempo period by a 12-bit value.
   * The difference between consecutive calls is used to 
   * recalculate the current tempo.
   */
  void adjust(uint16_t s){
    if(abs(speed-s) > 16){
      int64_t delta = (int64_t)samples*(speed-s)/2048;
      samples = max(int64_t(1), int64_t(samples+delta));
      speed = s;
    }
  }
  static AdjustableTapTempo* create(float sr, size_t limit){
    return new AdjustableTapTempo(sr, limit);
  }
  static void destroy(AdjustableTapTempo* obj){
    delete obj;
  }    
};

#endif /* __TapTempo_h__ */
