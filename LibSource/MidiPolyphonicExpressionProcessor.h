#ifndef __MidiPolyphonicExpressionProcessor_h__
#define __MidiPolyphonicExpressionProcessor_h__

/**
 * Implementation of MPE, MidiPolyphonicExpression, message processing.
 * We are controlling an AbstractSynth with:
 * X: Pitch Bend: setFrequency()
 * Y: CC74 or CC1 : setModulation()
 * Z: Channel Pressure : setPressure()
 */
template<class SynthVoice, int VOICES>
class MidiPolyphonicExpressionProcessor : public MidiProcessor {
// routes five per-note messages (Note On, Note Off, Pitch Bend, CC74 and Channel Pressure/Aftertouch)
// to the voice assigned to each channel
protected:
  SynthVoice* voice[VOICES];
  uint8_t notes[VOICES];
  float pressure[VOICES];
  float modulation[VOICES];
  uint8_t master_channel = 1;
  // One channel (usually Channel 1) is used for global messages
  // The global channel is set by the "MPE Zone." When the MPE Zone is the Lower Zone, Channel 1 is used for global messages. When the MPE Zone is Upper Zone is, Channel 16 is used for global messages.
  float zone_pitchbend = 0;
  float zone_pressure = 0;
  float zone_modulation = 0;
  float note_pitchbend_range = 48;
  float zone_pitchbend_range = 2;
  uint16_t rpn = 0;
public:
  MidiPolyphonicExpressionProcessor() {}
  virtual ~MidiPolyphonicExpressionProcessor(){};
  // class RpnMessage {
  // uint8_t ch;
  // uint16_t rpn;
  // uint16_t value;
  // };
  // RpnMessage getMCM(){
  // returns the MPE Configuration Message
  // ch0 cc 100:06 101:00 06:VOICES
  // }
  void noteOn(MidiMessage msg){
    uint8_t ch = getNoteChannel(msg);
    if(ch < VOICES)
      voice[ch]->noteOn(msg);
  }
  void noteOff(MidiMessage msg){
    uint8_t ch = getNoteChannel(msg);
    if(ch < VOICES)
      voice[ch]->noteOff(msg);
  }
  void setPitchBendRange(float range){
    zone_pitchbend_range = range;
  }    
  void controlChange(MidiMessage msg){
    switch(msg.getControllerNumber()){
    case MIDI_CC_MODULATION: // handle modulation same as CC74 for non-MPE compatibility
    /* case MIDI_CC_FREQUENCY_CUTOFF: { */
    case 74: {
// All MPE receivers are required to respond to CC #74 at the Zone and Note level. If a device receives CC #74
// on both a Master Channel and Member Channel, it must combine such data meaningfully and separately for
// each sounding note.
      uint8_t ch = getNoteChannel(msg);
      float value = msg.getControllerValue()/127.0f;
      if(ch < VOICES){
	voice[ch]->setModulation(zone_modulation + value);
	modulation[ch] = value;
      }else if(isMasterChannel(msg)){ // Zone message
	for(int i=0; i<VOICES; ++i)
	  voice[i]->setModulation(value + modulation[i]);
	zone_modulation = value;
      }
      break;
    }
    case 100: // RPN LSB comes first
      rpn = msg.getControllerValue();
      break;
    case 101: // RPN MSB
      rpn |= msg.getControllerValue()<<7;
      if(rpn == MIDI_RPN_RESET) // RPN reset
	rpn = 0;
      break;
    case 6: // Data Entry MSB
      if(rpn == MIDI_RPN_PITCH_BEND_RANGE){
	uint8_t semitones = msg.getControllerValue();
	if(isMasterChannel(msg)){
	  zone_pitchbend_range = semitones;
	}else{
	  note_pitchbend_range = semitones;
	}
      }else if(rpn == MIDI_RPN_MPE_CONFIGURATION){
	uint8_t n = msg.getChannel();
	uint8_t mm = msg.getControllerValue();
	// mm=0: MPE is Off (No Channels)
	// mm=1 to F: Assigns that number of MIDI Channels to the Zone
	if(mm != 0){
	  if(n == 0x0) // n=0: Lower Zone Master Channel
	    master_channel = 1;
	  else if(n == 0xf) // n=F: Upper Zone Master Channel
	    master_channel = 0xf;
	  // All other channel values are invalid and should be ignored
	}
	// Each Zone is activated with its own message, which can be sent in either order.
	// Sending an MCM with the number of Member Channels set to zero deactivates that Zone.
      }
      break;
    case 38: // Data Entry LSB
      if(rpn == 0){ // Pitch Bend Sensitivity
	// It is recommended that MPE devices use an integer number of semitones
	// for the range and either transmit the LSB as zero, or not transmit it
	// at all so that the receiver infers zero. MPE devices can still choose
	// to respond to 14-bit Pitch Bend Sensitivity messages for
	// compatibility with other equipment.
	float cents = msg.getControllerValue()/100.0f;
	if(isMasterChannel(msg)){
	  zone_pitchbend_range = ((int)zone_pitchbend_range) + cents;
	}else{
	  note_pitchbend_range = ((int)note_pitchbend_range) + cents;
	}
      }
      break;
    case MIDI_ALL_NOTES_OFF:
      allNotesOff();
      break;
    }
  }
  bool isMasterChannel(MidiMessage msg){
    return msg.getChannel()+1 == master_channel;
  }
  uint8_t getNoteChannel(MidiMessage msg){
    // The Lower Zone is controlled by Master Channel 1,
    // with Member Channels assigned sequentially from Channel 2 upwards.
    if(master_channel == 1)
      return (msg.getChannel()-1) % VOICES;
    else
      return (14-msg.getChannel()) % VOICES;
    // The Upper Zone is controlled by Master Channel 16,
    // with Member Channels assigned sequentially from Channel 15 downwards.
  }
  void pitchbend(MidiMessage msg){
    // Pitch Bend is both a Zone Message and a Note Level Message. If an MPE synthesizer receives Pitch Bend (for example) on both a Master and a Member Channel, it must combine the data meaningfully. The same is true for Channel Pressure. 
    uint8_t ch = getNoteChannel(msg);
    float value = note_pitchbend_range*msg.getPitchBend()/8192.0f;
    if(ch < VOICES){ // Note level message
      voice[ch]->setPitchBend(zone_pitchbend + value);
    }else if(isMasterChannel(msg)){ // Zone message
      float delta = value - zone_pitchbend;
      for(int i=0; i<VOICES; ++i)
	voice[i]->setPitchBend(voice[i]->getPitchBend()+delta);
      zone_pitchbend = value;
    }
  }
  void channelPressure(MidiMessage msg){
    uint8_t ch = getNoteChannel(msg);
    float value = msg.getChannelPressure()/127.0f;
    if(ch < VOICES){ // Note level message
      voice[ch]->setPressure(zone_pressure + value);
      pressure[ch] = value;
    }else if(isMasterChannel(msg)){ // Zone message
// All MPE receivers are required to respond to CC #74 at the Zone and Note level. If a device receives CC #74
// on both a Master Channel and Member Channel, it must combine such data meaningfully and separately for
// each sounding note.
      for(int i=0; i<VOICES; ++i)
	voice[i]->setPressure(value + pressure[i]);
      zone_pressure = value;
    }
  }
  void polyKeyPressure(MidiMessage msg){
    // Polyphonic Key Pressure may be used with notes on the Master Channel, but not on
    // other Channels
    if(isMasterChannel(msg)){
      uint8_t note = msg.getNote();
      for(int i=0; i<VOICES; ++i)
	if(voice[i]->getNote() == note)
	  voice[i]->setPressure(msg.getPolyKeyPressure()/127.0f);
    }
  }
  void allNotesOff() {
    for(int i=0; i<VOICES; ++i)
      voice[i]->gate(false);      
  }
  void setVoice(size_t index, SynthVoice* obj){
    if(index < VOICES)
      voice[index] = obj;
  }
  SynthVoice* getVoice(size_t index){
    if(index < VOICES)
      return voice[index];
    return NULL;
  }
  void setParameter(uint8_t parameter_id, float value){
    for(int i=0; i<VOICES; ++i)
      voice[i]->setParameter(parameter_id, value);
  }
};


template<class SynthVoice, int VOICES>
class MidiPolyphonicExpressionSignalGenerator : public MidiPolyphonicExpressionProcessor<SynthVoice, VOICES>, public SignalGenerator {
private:
  FloatArray buffer;
public:
  MidiPolyphonicExpressionSignalGenerator(FloatArray buffer) : buffer(buffer) {}
  virtual ~MidiPolyphonicExpressionSignalGenerator(){};
  float generate(){
    float sample = this->voice[0]->generate();
    for(int i=1; i<VOICES; ++i)
      sample += this->voice[i]->generate();
    return sample;
  }
  void generate(FloatArray output){
    this->voice[0]->generate(output);
    for(int i=1; i<VOICES; ++i){
      this->voice[i]->generate(buffer);
      output.add(buffer);
    }
  }
  static MidiPolyphonicExpressionSignalGenerator<SynthVoice, VOICES>* create(size_t blocksize){
    FloatArray buffer = FloatArray::create(blocksize);    
    return new MidiPolyphonicExpressionSignalGenerator<SynthVoice, VOICES>(buffer);
  }
  static void destroy(MidiPolyphonicExpressionSignalGenerator<SynthVoice, VOICES>* obj){
    FloatArray::destroy(obj->buffer);
    delete obj;
  }
};

template<class SynthVoice, int VOICES>
class MidiPolyphonicExpressionMultiSignalGenerator : public MidiPolyphonicExpressionProcessor<SynthVoice, VOICES>, public MultiSignalGenerator {
private:
  AudioBuffer* buffer;
public:
  MidiPolyphonicExpressionMultiSignalGenerator(AudioBuffer* buffer) : buffer(buffer) {}
  virtual ~MidiPolyphonicExpressionMultiSignalGenerator(){};
  static MidiPolyphonicExpressionMultiSignalGenerator<SynthVoice, VOICES>* create(size_t channels, size_t blocksize){
    AudioBuffer* buffer = AudioBuffer::create(channels, blocksize);    
    return new MidiPolyphonicExpressionMultiSignalGenerator<SynthVoice, VOICES>(buffer);
  }
  static void destroy(MidiPolyphonicExpressionMultiSignalGenerator<SynthVoice, VOICES>* obj){
    AudioBuffer::destroy(obj->buffer);
    delete obj;
  }
  void generate(AudioBuffer& output){
    this->voice[0]->generate(output);
    for(int i=1; i<VOICES; ++i){
      this->voice[i]->generate(*buffer);
      output.add(*buffer);
    }
  }  
};

#endif /* __MidiPolyphonicExpressionProcessor_h__ */
