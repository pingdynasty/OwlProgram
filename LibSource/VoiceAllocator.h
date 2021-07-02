#ifndef __VoiceAllocator_h__
#define __VoiceAllocator_h__

template<class SynthVoice, int VOICES>
class VoiceAllocator : public MidiProcessor {
protected:
  SynthVoice* voice[VOICES];
public:
  void allNotesOff() {
    for(int i=0; i<VOICES; ++i)
      voice[i]->gate(false);      
  }
  // todo: unison note on/off
  void allNotesOn() {
    for(int i=0; i<VOICES; ++i)
      voice[i]->gate(true);
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

template<class Allocator, class SynthVoice, int VOICES>
class VoiceAllocatorSignalGenerator : public Allocator, public SignalGenerator {
  static_assert(std::is_base_of<VoiceAllocator<SynthVoice, VOICES>, Allocator>::value, "Allocator must derive from VoiceAllocator");
  typedef VoiceAllocatorSignalGenerator<Allocator, SynthVoice, VOICES> MyClass;
protected:
  FloatArray buffer;
public:
  template <typename... Args>
  VoiceAllocatorSignalGenerator(FloatArray buffer, Args&&... args) :
    Allocator(std::forward<Args>(args)...), buffer(buffer) {}
  virtual ~VoiceAllocatorSignalGenerator(){};
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
  static MyClass* create(size_t blocksize){
    FloatArray buffer = FloatArray::create(blocksize);    
    return new MyClass(buffer);
  }
  static void destroy(MyClass* obj){
    FloatArray::destroy(obj->buffer);
    delete obj;
  }
};

template<class Allocator, class SynthVoice, int VOICES>
class VoiceAllocatorSignalProcessor : public Allocator, public SignalProcessor {
  static_assert(std::is_base_of<VoiceAllocator<SynthVoice, VOICES>, Allocator>::value, "Allocator must derive from VoiceAllocator");
  typedef VoiceAllocatorSignalProcessor<Allocator, SynthVoice, VOICES> MyClass;
protected:
  FloatArray buffer;
public:
  template <typename... Args>
  VoiceAllocatorSignalProcessor(FloatArray buffer, Args&&... args) :
    Allocator(std::forward<Args>(args)...), buffer(buffer) {}
  virtual ~VoiceAllocatorSignalProcessor(){};
  using MidiProcessor::process;
  float process(float input){
    float sample = 0;
    for(int i=0; i<VOICES; ++i)
      sample += this->voice[i]->process(input);
    return sample;
  }
  void process(FloatArray input, FloatArray output){
    for(int i=0; i<VOICES; ++i){
      this->voice[i]->process(input, buffer);
      output.add(buffer);
    }
  }
  static MyClass* create(size_t blocksize){
    FloatArray buffer = FloatArray::create(blocksize);    
    return new MyClass(buffer);
  }
  static void destroy(MyClass* obj){
    FloatArray::destroy(obj->buffer);
    delete obj;
  }
};

template<class Allocator, class SynthVoice, int VOICES>
class VoiceAllocatorMultiSignalGenerator : public Allocator, public MultiSignalGenerator {
  static_assert(std::is_base_of<VoiceAllocator<SynthVoice, VOICES>, Allocator>::value, "Allocator must derive from VoiceAllocator");
  typedef VoiceAllocatorMultiSignalGenerator<Allocator, SynthVoice, VOICES> MyClass;
protected:
  AudioBuffer* buffer;
public:
  template <typename... Args>
  VoiceAllocatorMultiSignalGenerator(AudioBuffer* buffer, Args&&... args) :
    Allocator(std::forward<Args>(args)...), buffer(buffer) {}
  virtual ~VoiceAllocatorMultiSignalGenerator(){};
  void generate(AudioBuffer& output){
    this->voice[0]->generate(output);
    for(int i=1; i<VOICES; ++i){
      this->voice[i]->generate(*buffer);
      output.add(*buffer);
    }
  }  
  static MyClass* create(size_t channels, size_t blocksize){
    AudioBuffer* buffer = AudioBuffer::create(channels, blocksize);
    return new MyClass(buffer);
  }
  static void destroy(MyClass* obj){
    AudioBuffer::destroy(obj->buffer);
    delete obj;
  }
};

template<class Allocator, class SynthVoice, int VOICES>
class VoiceAllocatorMultiSignalProcessor : public Allocator, public MultiSignalProcessor {
  static_assert(std::is_base_of<VoiceAllocator<SynthVoice, VOICES>, Allocator>::value, "Allocator must derive from VoiceAllocator");
  typedef VoiceAllocatorMultiSignalProcessor<Allocator, SynthVoice, VOICES> MyClass;
protected:
  AudioBuffer* buffer;
public:
  template <typename... Args>
  VoiceAllocatorMultiSignalProcessor(AudioBuffer* buffer, Args&&... args) :
    Allocator(std::forward<Args>(args)...), buffer(buffer) {}
  virtual ~VoiceAllocatorMultiSignalProcessor(){};
  using MidiProcessor::process;
  void process(AudioBuffer& input, AudioBuffer& output){
    for(int i=0; i<VOICES; ++i){
      this->voice[i]->process(input, *buffer);
      output.add(*buffer);
    }
  }  
  static MyClass* create(size_t channels, size_t blocksize){
    AudioBuffer* buffer = AudioBuffer::create(channels, blocksize);
    return new MyClass(buffer);
  }
  static void destroy(MyClass* obj){
    AudioBuffer::destroy(obj->buffer);
    delete obj;
  }
};


#endif /* __VoiceAllocator_h__ */
