#include "FloatArray.h"
#include "basicmaths.h"
#include "message.h"
#include <string.h>

void FloatArray::getMin(float* value, int* index){
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX
  uint32_t idx;
  arm_min_f32(data, size, value, &idx);
  *index = (int)idx;
#else
  *value=data[0];
  *index=0;
  for(size_t n=1; n<size; n++){
    float currentValue=data[n];
    if(currentValue<*value){
      *value=currentValue;
      *index=n;
    }
  }
#endif
}

float FloatArray::getMinValue(){
  float value;
  int index;
  /// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  getMin(&value, &index);
  return value;
}

int FloatArray::getMinIndex(){
  float value;
  int index;
  /// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  getMin(&value, &index);
  return index;
}

void FloatArray::getMax(float* value, int* index){
  ASSERT(size>0, "Wrong size");
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX 
  uint32_t idx;
  arm_max_f32(data, size, value, &idx);
  *index = (int)idx;
#else
  *value=data[0];
  *index=0;
  for(size_t n=1; n<size; n++){
    float currentValue=data[n];
    if(currentValue>*value){
      *value=currentValue;
      *index=n;
    }
  }
#endif
}

float FloatArray::getMaxValue(){
  float value;
  int index;
  /// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  getMax(&value, &index);
  return value;
}

int FloatArray::getMaxIndex(){
  float value;
  int index;
  /// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  getMax(&value, &index);
  return index;
}

void FloatArray::rectify(FloatArray& destination){ //this is actually "copy data with rectifify"
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX   
  arm_abs_f32(data, destination.getData(), size);
#else
  size_t minSize= min(size,destination.getSize()); //TODO: shall we take this out and allow it to segfault?
  for(size_t n=0; n<minSize; n++){
    destination[n] = fabsf(data[n]);
  }
#endif  
}

void FloatArray::reverse(FloatArray& destination){ //this is actually "copy data with reverse"
  if(destination==*this){ //make sure it is not called "in-place"
    reverse();
    return;
  }
  for(size_t n=0; n<size; n++){
    destination[n]=data[size-n-1];
  }
}

void FloatArray::reverse(){//in place
  for(size_t n=0; n<size/2; n++){
    float temp=data[n];
    data[n]=data[size-n-1];
    data[size-n-1]=temp;
  }
}

void FloatArray::reciprocal(FloatArray& destination){
  float* data = getData();
  for(size_t n=0; n<getSize(); n++)
    destination[n] = 1.0f/data[n];
}

float FloatArray::getRms(){
  float result;
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX  
  arm_rms_f32 (data, size, &result);
#else
  result=0;
  float *pSrc= data;
  for(size_t n=0; n<size; n++){
    result += pSrc[n]*pSrc[n];
  }
  result=sqrtf(result/size);
#endif
  return result;
}

float FloatArray::getSum(){
  float result = 0;
  for(size_t n=0; n<size; n++)
    result += data[n];
  return result;
} 

float FloatArray::getMean(){
  float result;
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX  
  arm_mean_f32 (data, size, &result);
#else
  result=0;
  for(size_t n=0; n<size; n++){
    result+=data[n];
  }
  result=result/size;
#endif
  return result;
}

float FloatArray::getPower(){
  float result;
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX  
  arm_power_f32 (data, size, &result);
#else
  result=0;
  float *pSrc = data;
  for(size_t n=0; n<size; n++){
    result += pSrc[n]*pSrc[n];
  }
#endif
  return result;
}

float FloatArray::getStandardDeviation(){
  float result;
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX  
  arm_std_f32 (data, size, &result);
#else
  result=sqrtf(getVariance());
#endif
  return result;
}

float FloatArray::getVariance(){
  float result;
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX  
  arm_var_f32(data, size, &result);
#else
  float sumOfSquares=getPower();
  float sum=0;
  for(size_t n=0; n<size; n++){
    sum+=data[n];
  }
  result=(sumOfSquares - sum*sum/size) / (size - 1);
#endif
  return result;
}

void FloatArray::clip(){
  clip(1);
}

void FloatArray::clip(float max){
  for(size_t n=0; n<size; n++){
    if(data[n]>max)
      data[n]=max;
    else if(data[n]<-max)
      data[n]=-max;
  }
}

void FloatArray::clip(float min, float max){
  for(size_t n=0; n<size; n++){
    if(data[n]>max)
      data[n]=max;
    else if(data[n]<min)
      data[n]=min;
  }
}

FloatArray FloatArray::subArray(int offset, size_t length){
  ASSERT(size >= offset+length, "Array too small");
  return FloatArray(data+offset, length);
}

void FloatArray::setAll(float value){
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX
  arm_fill_f32(value, data, size);
#else
  for(size_t n=0; n<size; n++){
    data[n]=value;
  }
#endif /* ARM_CORTEX */
}

void FloatArray::add(FloatArray operand2, FloatArray destination){ //allows in-place
  ASSERT(operand2.size >= size &&  destination.size<=size, "Arrays must be matching size");
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX
  /* despite not explicitely documented in the CMSIS documentation,
      this has been tested to behave properly even when pSrcA==pDst
      void 	arm_add_f32 (float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
  */
  arm_add_f32(data, operand2.data, destination.data, size);
#else
  for(size_t n=0; n<size; n++){
    destination[n]=data[n]+operand2[n];
  }
#endif /* ARM_CORTEX */
}

void FloatArray::add(FloatArray operand2){ //in-place
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  add(operand2, *this);
}

void FloatArray::add(float scalar){
  for(size_t n=0; n<size; n++){
    data[n] += scalar;
  } 
}

void FloatArray::add(float scalar, FloatArray destination){
  for(size_t n=0; n<size; n++)
    destination[n] = data[n]+scalar;
}

void FloatArray::subtract(FloatArray operand2, FloatArray destination){ //allows in-place
  ASSERT(operand2.size == size && destination.size >= size, "Arrays size mismatch");
  /// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX
  /* despite not explicitely documented in the CMSIS documentation,
      this has been tested to behave properly even when pSrcA==pDst
      void 	arm_sub_f32 (float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
  */
  arm_sub_f32(data, operand2.data, destination.data, size);
  #else
  for(size_t n=0; n<size; n++){
    destination[n]=data[n]-operand2[n];
  }
  #endif /* ARM_CORTEX */
}

void FloatArray::subtract(FloatArray operand2){ //in-place
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  subtract(operand2, *this);
}

void FloatArray::subtract(float scalar){
  for(size_t n=0; n<size; n++){
    data[n]-=scalar;
  } 
}

void FloatArray::multiply(FloatArray operand2, FloatArray destination){ //allows in-place
  ASSERT(operand2.size == size &&  destination.size==size, "Arrays must be same size");
  /// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX
  /* despite not explicitely documented in the CMSIS documentation,
      this has been tested to behave properly even when pSrcA==pDst
      void 	arm_mult_f32 (float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
  */
    arm_mult_f32(data, operand2.data, destination, size);
  #else
  for(size_t n=0; n<size; n++){
    destination[n]=data[n]*operand2[n];
  }

  #endif /* ARM_CORTEX */
}

void FloatArray::multiply(FloatArray operand2){ //in-place
  /// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  multiply(operand2, *this);
}

void FloatArray::multiply(float scalar){
#ifdef ARM_CORTEX
  arm_scale_f32(data, scalar, data, size);
#else
  for(size_t n=0; n<size; n++)
    data[n]*=scalar;
#endif
}

void FloatArray::multiply(float scalar, FloatArray destination){
#ifdef ARM_CORTEX
  arm_scale_f32(data, scalar, destination, size);
#else
  for(size_t n=0; n<size; n++)
    destination[n] = data[n] * scalar;
#endif
}

void FloatArray::negate(FloatArray& destination){//allows in-place
#ifdef ARM_CORTEX
  arm_negate_f32(data, destination.getData(), size); 
#else
  for(size_t n=0; n<size; n++){
    destination[n]=-data[n];
  }
#endif /* ARM_CORTEX */
}

void FloatArray::noise(){
  noise(-1, 1);
}

void FloatArray::noise(float min, float max){
  float amplitude = fabsf(max-min);
  float offset = min;
  for(size_t n=0; n<size; n++)
    data[n] = randf() * amplitude + offset;
}


void FloatArray::convolve(FloatArray operand2, FloatArray destination){
  ASSERT(destination.size >= size + operand2.size -1, "Destination array too small");
#ifdef ARM_CORTEX
  arm_conv_f32(data, size, operand2.data, operand2.size, destination);
#else
  size_t size2 = operand2.getSize();
  for(size_t n=0; n<size+size2-1; n++){
    size_t n1 = n;
    destination[n] = 0;
    for(size_t k=0; k<size2; k++){
      if(n1>=0 && n1<size)
        destination[n] += data[n1]*operand2[k];
      n1--;
    }
  }
#endif /* ARM_CORTEX */
}

void FloatArray::convolve(FloatArray operand2, FloatArray destination, int offset, size_t samples){
  ASSERT(destination.size >= size + operand2.size -1, "Destination array too small"); //TODO: change this condition to the actual size being written(will be samples+ tail)
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX
  //TODO: I suspect a bug in arm_conv_partial_f32
  //it seems that destination[n] is left unchanged for n<offset
  //and the result is actually stored from destination[offset] onwards
  //that is, in the same position where they would be if a full convolution was performed.
  //This requires (destination.size >= size + operand2.size -1). Ideally you would want destination to be smaller
  arm_conv_partial_f32(data, size, operand2.data, operand2.size, destination.getData(), offset, samples);
#else
  //this implementations reproduces the (buggy?) behaviour of arm_conv_partial (see comment above and inline comments below)
  /*
  This implementation is just a copy/paste/edit from the overloaded method
  */
  size_t size2=operand2.getSize();
  for (size_t n=offset; n<offset+samples; n++){
    size_t n1=n;
    destination[n] =0; //this should be [n-offset]
    for(size_t k=0; k<size2; k++){
      if(n1>=0 && n1<size)
        destination[n]+=data[n1]*operand2[k];//this should be destination[n-offset]
      n1--;
    }
  }
#endif /* ARM_CORTEX */
}

void FloatArray::correlate(FloatArray operand2, FloatArray destination){ 
  destination.setAll(0);
  /// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  correlateInitialized(operand2, destination);
}

void FloatArray::correlateInitialized(FloatArray operand2, FloatArray destination){
  ASSERT(destination.size >= size+operand2.size-1, "Destination array too small"); //TODO: change CMSIS docs, which state a different size
/// @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
#ifdef ARM_CORTEX
  arm_correlate_f32(data, size, operand2.data, operand2.size, destination);
#else
  //correlation is the same as a convolution where one of the signals is flipped in time
  //so we flip in time operand2 
  operand2.reverse();
  //and convolve it with fa to obtain the correlation
  convolve(operand2, destination);
  //and we flip back operand2, so that the input is not modified
  operand2.reverse();
#endif /* ARM_CORTEX */  
}

void FloatArray::gainToDecibel(FloatArray destination){
  ASSERT(destination.getSize()>=size, "Wrong array size");
  for(size_t i=0; i<size; i++)
    destination[i] = log10f(data[i])*20.0;
}

void FloatArray::decibelToGain(FloatArray destination){
  ASSERT(destination.getSize()>=size, "Wrong array size");
  for(size_t i=0; i<size; i++)
    destination[i] = exp10f(data[i]*0.05);
}

void FloatArray::ramp(float from, float to){
  float step = (to-from)/size;
  for(size_t i=0; i<size; i++){
    data[i] = from;
    from += step;
  }
}

void FloatArray::scale(float from, float to, FloatArray destination){
  float step = (to-from)/size;
  for(size_t i=0; i<size; i++){
    data[i] *= from;
    from += step;
  }  
}

/*
 * Third-order static soft-clipping function.
 * ref:  T. Araya and A. Suyama, “Sound effector capable of
 * imparting plural sound effects like distortion and other
 * effects,” US Patent 5,570,424, 29 Oct. 1996.
 */
void FloatArray::softclip(FloatArray destination){
  for(size_t i=0; i<size; i++){
    float x = data[i];
    destination[i] = clamp((3*x/2)*(1-x*x/3), -1.0f, 1.0f);
  }
}

void FloatArray::tanh(FloatArray destination){
  for(size_t i=0; i<size; i++)
    destination[i] = tanhf(data[i]);
}

FloatArray FloatArray::create(int size){
  FloatArray fa(new float[size], size);
  fa.clear();
  return fa;
}

void FloatArray::destroy(FloatArray array){
  delete[] array.data;
}
