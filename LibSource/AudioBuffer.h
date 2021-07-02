#ifndef __AudioBuffer_h__
#define __AudioBuffer_h__

#include "FloatArray.h"

class AudioBuffer {
public:
  virtual ~AudioBuffer();
  virtual FloatArray getSamples(int channel) = 0;
  virtual int getChannels() = 0;
  virtual int getSize() = 0;
  virtual void clear() = 0;
  void multiply(float scalar){
    for(int i=0; i<getChannels(); ++i)
      getSamples(i).multiply(scalar);
  }
  void add(float scalar){
    for(int i=0; i<getChannels(); ++i)
      getSamples(i).add(scalar);
  }
  void add(AudioBuffer& other){
    for(int i=0; i<getChannels(); ++i)
      getSamples(i).add(other.getSamples(i));
  }
  void copyFrom(AudioBuffer& other){
    for(int i=0; i<getChannels(); ++i)
      getSamples(i).copyFrom(other.getSamples(i));
  }
  void copyTo(AudioBuffer& other){
    for(int i=0; i<getChannels(); ++i)
      getSamples(i).copyTo(other.getSamples(i));
  }
  static AudioBuffer* create(int channels, int samples);
  static void destroy(AudioBuffer* buffer);
};

#endif // __AudioBuffer_h__
