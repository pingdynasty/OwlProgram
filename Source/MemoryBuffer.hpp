#include "Patch.h"
#include "message.h"
#include <string.h>
#include <stdlib.h>

class MemoryBuffer : public AudioBuffer {
protected:
  float* buffer;
  size_t channels;
  size_t size;
public:
  MemoryBuffer(float* buf, size_t ch, size_t sz): buffer(buf), channels(ch), size(sz) {}
  virtual ~MemoryBuffer(){}
  void clear() override {
    memset(buffer, 0, size*channels*sizeof(float));
  }
  FloatArray getSamples(int channel) override {
    ASSERT((size_t)channel < channels, "Invalid channel");
    return FloatArray(buffer+channel*size, size);
  }
  int getChannels() override {
    return channels;
  }
  int getSize() override {
    return size;
  }
};

class ManagedMemoryBuffer : public MemoryBuffer {
public:
  ManagedMemoryBuffer(size_t ch, size_t sz) :
    MemoryBuffer(new float[ch*sz], ch, sz) {
    if(buffer == NULL)
      error(OUT_OF_MEMORY_ERROR_STATUS, "Out of memory");
  }
  ~ManagedMemoryBuffer(){
    delete[] buffer;
  }
};
