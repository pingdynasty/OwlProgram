#ifndef _InterpolatingCircularBuffer_hpp_
#define _InterpolatingCircularBuffer_hpp_

#include <stdint.h>
#include "CircularBuffer.h"
#include "Interpolator.h"

template<InterpolationMethod im = LINEAR_INTERPOLATION>
class InterpolatingCircularFloatBuffer : public CircularBuffer<float> {
protected:
  float delay_samples = 0;
public:
  InterpolatingCircularFloatBuffer() {}
  InterpolatingCircularFloatBuffer(float* data, size_t size): CircularFloatBuffer(data, size) {}
  /**
   * Interpolated write at sub-sample index.
   * Inserts a value linearly interpolated at a fractional index.
   */
  void writeAt(float index, float value);

  /**
   * Interpolated read at sub-sample index.
   * @return a value linearly interpolated at a fractional index
   */
  float readAt(float index);

  float getFractionalDelay(){
    return delay_samples;
  }

  /**
   * Read with a fractional delay
   */
  void delay(float* out, size_t len, float delay){
    // float pos = fmodf(writepos-delay+size, size);
    float pos = writepos - delay + size;
    while(len--){
      *out++ = readAt(pos);
      pos += 1;
    }
    delay_samples = delay;
  }  
  
  /**
   * Write to buffer and read with a fractional delay
   * @note input buffer length is added to delay time
   */
  void delay(float* in, float* out, size_t len, float readDelay){
    write(in, len);
    delay(out, len, readDelay + len); // set delay relative to where we started writing
  }
  
  /**
   * Read with a delay that ramps up or down
   * from @param beginDelay to @param endDelay
   */
  void delay(float* out, size_t len, float beginDelay, float endDelay){
    // float pos = fmodf(writepos-beginDelay+size, size);
    float pos = writepos - beginDelay + size;
    float incr = (len+endDelay-beginDelay)/len;
    // incr = clamp(incr, 0.5, 2);
    // delay_samples = beginDelay + incr*len - len;
    delay_samples = endDelay;
    while(len--){
      *out++ = readAt(pos);
      pos += incr;
    }
  }
  
  /**
   * Write to buffer and read with a delay that ramps up or down
   * from @param beginDelay to @param endDelay
   * @note input buffer length is added to delay times
   */
  void delay(float* in, float* out, size_t len, float beginDelay, float endDelay){
    write(in, len);
    delay(out, len, beginDelay + len, endDelay + len); // set delays relative to where we started writing
  }

  static InterpolatingCircularFloatBuffer<im>* create(size_t len){
    InterpolatingCircularFloatBuffer<im>* obj = new InterpolatingCircularFloatBuffer<im>(new float[len], len);
    obj->clear();
    return obj;
  }

  static void destroy(InterpolatingCircularFloatBuffer<im>* obj){
    delete[] obj->data;
    delete obj;
  }
};

template<>
float InterpolatingCircularFloatBuffer<LINEAR_INTERPOLATION>::readAt(float index){
  size_t idx = (size_t)index;
  float low = CircularFloatBuffer::readAt(idx);
  float high = CircularFloatBuffer::readAt(idx+1);
  float frac = index - idx;
  return Interpolator::linear(low, high, frac);
}

template<>
void InterpolatingCircularFloatBuffer<LINEAR_INTERPOLATION>::writeAt(float index, float value){
  size_t idx = (size_t)index;
  float low = CircularFloatBuffer::readAt(idx);
  float high = CircularFloatBuffer::readAt(idx+1);
  float frac = index - idx;
  writeAt(idx, low + (value-low)*frac);
  writeAt(idx+1, value + (high-value)*frac);
}

template<>
float InterpolatingCircularFloatBuffer<COSINE_INTERPOLATION>::readAt(float index){
  size_t idx = (size_t)index;
  float low = CircularFloatBuffer::readAt(idx);
  float high = CircularFloatBuffer::readAt(idx+1);
  float frac = index - idx;
  return Interpolator::cosine(low, high, frac);
}

template<>
float InterpolatingCircularFloatBuffer<CUBIC_3P_INTERPOLATION>::readAt(float index){
  size_t idx = (size_t)index;
  float y0 = CircularFloatBuffer::readAt(idx-1);
  float y1 = CircularFloatBuffer::readAt(idx);
  float y2 = CircularFloatBuffer::readAt(idx+1);
  return Interpolator::cubic(y0, y1, y2, index - idx);
}

template<>
float InterpolatingCircularFloatBuffer<CUBIC_4P_INTERPOLATION>::readAt(float index){
  size_t idx = (size_t)index;
  float y0 = CircularFloatBuffer::readAt(idx-1);
  float y1 = CircularFloatBuffer::readAt(idx);
  float y2 = CircularFloatBuffer::readAt(idx+1);
  float y3 = CircularFloatBuffer::readAt(idx+2);
  return Interpolator::cubic(y0, y1, y2, y3, index - idx);
}

template<>
float InterpolatingCircularFloatBuffer<CUBIC_4P_SMOOTH_INTERPOLATION>::readAt(float index){
  size_t idx = (size_t)index;
  float y0 = CircularFloatBuffer::readAt(idx-1);
  float y1 = CircularFloatBuffer::readAt(idx);
  float y2 = CircularFloatBuffer::readAt(idx+1);
  float y3 = CircularFloatBuffer::readAt(idx+2);
  return Interpolator::cubicSmooth(y0, y1, y2, y3, index - idx);
}

template<>
float InterpolatingCircularFloatBuffer<HERMITE_INTERPOLATION>::readAt(float index){
  size_t idx = (size_t)index;
  float y0 = CircularFloatBuffer::readAt(idx-1);
  float y1 = CircularFloatBuffer::readAt(idx);
  float y2 = CircularFloatBuffer::readAt(idx+1);
  float y3 = CircularFloatBuffer::readAt(idx+2);
  return Interpolator::hermite(y0, y1, y2, y3, index - idx);
}

#endif /* _InterpolatingCircularBuffer_hpp_ */
