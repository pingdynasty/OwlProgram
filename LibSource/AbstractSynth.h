#ifndef __AbstractSynth_h__
#define __AbstractSynth_h__

#include <climits>
#include "Synth.h"
#include "MidiProcessor.h"
#include "VelocityCurve.h"

class AbstractSynth : public Synth, public MidiProcessor, public VelocityCurve {
protected:
  uint8_t note = 60; // C4
  float pb = 0;
  float pb_range = 2;
public:
  virtual ~AbstractSynth(){}
  /**
   * Set note in whole semitones
   */
  uint8_t getNote(){
    return note;
  }
  /**
   * Set note in whole semitones and update frequency
   */
  virtual void setNote(uint8_t note){
    this->note = note;
    setFrequency(noteToFrequency(note+pb));
  }
  /**
   * Get pitch bend amount in semitones
   */
  float getPitchBend(){
    return pb;
  }
  /**
   * Set pitch bend amount in semitones and update frequency
   */
  virtual void setPitchBend(float pb){
    this->pb = pb;
    setFrequency(noteToFrequency(note+pb));
  }
  /**
   * Get pitch bend range in semitones
   */
  float getPitchBendRange(){
    return pb_range;
  }
  /**
   * Set pitch bend range in semitones.
   * Does not update the frequency; effective from next pitch bend change
   */
  void setPitchBendRange(float range){
    this->pb_range = range;
  }  
  // MIDI handlers
  virtual void noteOn(MidiMessage msg) override {
    setNote(msg.getNote());
    setFrequency(noteToFrequency(note+pb));
    setGain(velocityToGain(msg.getVelocity()));
    gate(true);
  }
  virtual void noteOff(MidiMessage msg) override {
    gate(false);
  }
  virtual void controlChange(MidiMessage msg) override {
    if(msg.getControllerNumber() == MIDI_CC_MODULATION)
      setModulation(msg.getControllerValue()/128.0f);
    else if(msg.getControllerNumber() == MIDI_ALL_NOTES_OFF)
      allNotesOff();
  }
  virtual void channelPressure(MidiMessage msg) override {
    setPressure(msg.getChannelPressure()/128.0f);
  }
  virtual void polyKeyPressure(MidiMessage msg) override {
    setPressure(msg.getPolyKeyPressure()/128.0f);
  }
  virtual void setModulation(float modulation){} // default implementation does nothing
  virtual void setPressure(float pressure){}
  virtual void pitchbend(MidiMessage msg) override {
    setPitchBend(pb_range*msg.getPitchBend()/8192.0f);
  }
  virtual void allNotesOff(){
    gate(false);
  }
  // static utility methods
  static inline float frequencyToNote(float freq){
    return 12 * log2f(freq / 440) + 69;
  }
  static inline float noteToFrequency(float note){
    return 440 * exp2f((note - 69) / 12);
  }
};

#endif // __AbstractSynth_h__
