#ifndef _CrossFadingCircularBuffer_hpp_
#define _CrossFadingCircularBuffer_hpp_

#include <stdint.h>
#include "CircularBuffer.h"

template<typename T>
class CrossFadingCircularBuffer : public CircularBuffer<T> {
protected:
  FloatArray buffer;
public:
  CrossFadingCircularBuffer() {}
  CrossFadingCircularBuffer(T* data, size_t size, FloatArray buffer):
    CircularBuffer<T>(data, size), buffer(buffer) {}  
  /**
   * Read from buffer with a delay.
   * The output will be crossfaded between the previous delay time and the new one.
   */
  void delay(T* out, size_t len, int delay_samples){
    delay(out, len, CircularBuffer<T>::getDelay(), delay_samples);
  }
  /**
   * Write to buffer and read with a delay
   */
  void delay(T* in, T* out, size_t len, int delay_samples){
    CircularBuffer<T>::write(in, len);
    delay(out, len, CircularBuffer<T>::getDelay(), delay_samples + len);
  }
  /**
   * Read from buffer with a delay
   */
  void delay(T* out, size_t len, int beginDelay, int endDelay){
    ASSERT(len <= buffer.getSize(), "Buffer too small");
    FloatArray output(out, len);
    CircularBuffer<T>::setDelay(beginDelay);
    CircularBuffer<T>::read(out, len);
    output.scale(1, 0);
    CircularBuffer<T>::setDelay(endDelay);
    CircularBuffer<T>::read(buffer.getData(), len);
    buffer.scale(0, 1);
    output.add(buffer);
  }
  /**
   * Write to buffer and read with a delay
   */
  void delay(T* in, T* out, size_t len, int beginDelay, int endDelay){
    CircularBuffer<T>::write(in, len);
    delay(out, len, beginDelay + len, endDelay + len); // set delays relative to where we started writing
  }
  static CrossFadingCircularBuffer<T>* create(size_t len, size_t blocksize){
    CrossFadingCircularBuffer<T>* obj =
      new CrossFadingCircularBuffer<T>(new T[len], len, FloatArray::create(blocksize));
    obj->clear();
    return obj;
  }
  static void destroy(CrossFadingCircularBuffer<T>* obj){
    FloatArray::destroy(obj->buffer);
    delete[] obj->data;
    delete obj;
  }
};

typedef CrossFadingCircularBuffer<float> CrossFadingCircularFloatBuffer;

#endif /* _CrossFadingCircularBuffer_hpp_ */
