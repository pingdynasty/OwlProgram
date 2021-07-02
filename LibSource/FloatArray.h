#ifndef __FloatArray_h__
#define __FloatArray_h__

#include <cstddef>
#include "SimpleArray.h"

/**
 * This class contains useful methods for manipulating arrays of floats.
 * It also provides a convenient handle to the array pointer and the size of the array.
 * FloatArray objects can be passed by value without copying the contents of the array.
 */
class FloatArray : public SimpleArray<float> {
public:
  FloatArray(){}
  FloatArray(float* data, size_t size) :
    SimpleArray(data, size) {}

  /**
   * Set all the values in the array.
   * Sets all the elements of the array to **value**.
   * @param[in] value all the elements are set to this value.
  */
  void setAll(float value);

  /**
   * Clear the array.
   * Set all the values in the array to 0.
  */
  void clear(){
    setAll(0);
  }
  
  /**
   * Get the minimum value in the array and its index
   * @param[out] value will be set to the minimum value after the call
   * @param[out] index will be set to the index of the minimum value after the call
   * 
   */
  void getMin(float* value, int* index);
  
  /**
   * Get the maximum value in the array and its index
   * @param[out] value will be set to the maximum value after the call
   * @param[out] index will be set to the index of the maximum value after the call
  */
  void getMax(float* value, int* index);
  
  /**
   * Get the minimum value in the array
   * @return the minimum value contained in the array
  */
  float getMinValue();
  
  /**
   * Get the maximum value in the array
   * @return the maximum value contained in the array
   */
  float getMaxValue();
  
  /**
   * Get the index of the minimum value in the array
   * @return the mimimum value contained in the array
   */
  int getMinIndex();
  
  /**
   * Get the index of the maximum value in the array
   * @return the maximum value contained in the array
   */
  int getMaxIndex();
  
  /**
   * Absolute value of the array.
   * Stores the absolute value of the elements in the array into destination.
   * @param[out] destination the destination array.
  */
  void rectify(FloatArray& destination);
  
  /**
   * Absolute value of the array, in-place version.
   * Sets each element in the array to its absolute value.
   */
  void rectify(){
    rectify(*this);
  }
  
  /**
   * Reverse the array
   * Copies the elements of the array in reversed order into destination.
   * @param[out] destination the destination array.
  */
  void reverse(FloatArray& destination);
  
  /**
   * Reverse the array.
   * Reverses the order of the elements in the array.
  */
  void reverse(); //in place
  
  /**
   * Reciprocal of the array.
   * Stores the reciprocal of the elements in the array into destination.
   * @param[out] destination the destination array.
  */
  void reciprocal(FloatArray& destination);
  
  /**
   * Reciprocal of the array, in-place version.
   * Sets each element in the array to its reciprocal.
  */
  void reciprocal(){
    reciprocal(*this);
  }
  
  /**
   * Negate the array.
   * Stores the opposite of the elements in the array into destination.
   * @param[out] destination the destination array.
   * @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
   */
  void negate(FloatArray& destination);
  
  /**
   * Negate the array.
   * Sets each element in the array to its opposite.
  */
  void negate(){
    negate(*this);
  }
  
  /**
   * Random values
   * Fills the array with random values in the range [-1, 1)
   */
  void noise();
  
  /**
   * Random values in range.
   * Fills the array with random values in the range [**min**, **max**)
   * @param min minimum value in the range
   * @param max maximum value in the range 
   */
  void noise(float min, float max);
  
  /**
   * Root mean square value of the array.
   * Gets the root mean square of the values in the array.
  */
  float getRms();
  
  /**
   * Mean of the array.
   * Gets the mean (or average) of the values in the array.
  */
  float getMean();
  
  /**
   * Sum of the array.
   * Gets the sum of the values in the array.
  */
  float getSum();
  
  /**
   * Power of the array.
   * Gets the power of the values in the array.
  */
  float getPower();
  
  /**
   * Standard deviation of the array.
   * Gets the standard deviation of the values in the array.
  */
  float getStandardDeviation();
  
  /**
   * Variance of the array.
   * Gets the variance of the values in the array.
  */
  float getVariance();

  /**
   * Clips the elements in the array in the range [-1, 1].
  */
  void clip();
  
  /**
   * Clips the elements in the array in the range [-**range**, **range**].
   * @param range clipping value.
  */
  void clip(float range);
  
  /**
   * Clips the elements in the array in the range [**min**, **max**].
   * @param min minimum value
   * @param max maximum value
  */
  void clip(float min, float max);

  /**
   * Applies a cubic soft-clip algorithm to all elements in the array which limits them to the range [-1, 1]
   * @param[out] destination the destination array
   */
  void softclip(FloatArray destination);

  /**
   * Applies a cubic soft-clip algorithm to all elements in the array which limits them to the range [-1, 1]
   * In-place version.
   */
  void softclip(){
    softclip(*this);
  }  
  
  /**
   * Element-wise sum between arrays.
   * Sets each element in **destination** to the sum of the corresponding element of the array and **operand2**
   * @param[in] operand2 second operand for the sum
   * @param[out] destination the destination array
  */
  void add(FloatArray operand2, FloatArray destination);
  
  /**
   * Element-wise sum between arrays.
   * Adds each element of **operand2** to the corresponding element in the array.
   * @param operand2 second operand for the sum
  */
  void add(FloatArray operand2); //in-place
  
  /**
   * Array-scalar addition.
   * Adds **scalar** to each value in the array and put the result in **destination**
   * @param[in] scalar value to be added to the array
   * @param[out] destination the destination array
   */
  void add(float scalar, FloatArray destination);

  /**
   * In-place array-scalar addition.
   * Adds **scalar** to each value in the array
   * @param[in] scalar value to be added to the array
  */
  void add(float scalar);

  /**
   * Element-wise difference between arrays.
   * Sets each element in **destination** to the difference between the corresponding element of the array and **operand2**
   * @param[in] operand2 second operand for the subtraction
   * @param[out] destination the destination array
  */
  void subtract(FloatArray operand2, FloatArray destination);
  
  
  /**
   * Element-wise difference between arrays.
   * Subtracts from each element of the array the corresponding element in **operand2**.
   * @param[in] operand2 second operand for the subtraction
  */
  void subtract(FloatArray operand2); //in-place
  
  /**
   * Array-scalar subtraction.
   * Subtracts **scalar** from the values in the array.
   * @param scalar to be subtracted from the array
  */
  void subtract(float scalar);
  
/**
   * Element-wise multiplication between arrays.
   * Sets each element in **destination** to the product of the corresponding element of the array and **operand2**
   * @param[in] operand2 second operand for the product
   * @param[out] destination the destination array
  */
  void multiply(FloatArray operand2, FloatArray destination);
  
   /**
   * Element-wise multiplication between arrays.
   * Multiplies each element in the array by the corresponding element in **operand2**.
   * @param operand2 second operand for the sum
  */
  void multiply(FloatArray operand2); //in-place
  
  /**
   * Array-scalar multiplication.
   * Multiplies the values in the array by **scalar**.
   * @param scalar to be multiplied with the array elements
  */
  void multiply(float scalar);
  
  /**
   * Array-scalar multiplication.
   * Multiplies the values in the array by **scalar**.
   * @param scalar to be subtracted from the array
   * @param destination the destination array
  */
  void multiply(float scalar, FloatArray destination);

  /**
   * Convolution between arrays.
   * Sets **destination** to the result of the convolution between the array and **operand2**
   * @param[in] operand2 the second operand for the convolution
   * @param[out] destination array. It must have a minimum size of this+other-1.
   * @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
   */
  void convolve(FloatArray operand2, FloatArray destination);
  
  /** 
   * Partial convolution between arrays.
   * Perform partial convolution: start at **offset** and compute **samples** values.
   * @param[in] operand2 the second operand for the convolution.
   * @param[out] destination the destination array.
   * @param[in] offset first output sample to compute
   * @param[in] samples number of samples to compute
   * @remarks **destination[n]** is left unchanged for n<offset and the result is stored from destination[offset] onwards
   * that is, in the same position where they would be if a full convolution was performed.
   * @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  */
  void convolve(FloatArray operand2, FloatArray destination, int offset, size_t samples);
  
  /** 
   * Correlation between arrays.
   * Sets **destination** to the correlation of the array and **operand2**.
   * @param[in] operand2 the second operand for the correlation
   * @param[out] destination the destination array. It must have a minimum size of 2*max(srcALen, srcBLen)-1
   * @note When built for ARM Cortex-M processor series, this method uses the optimized <a href="http://www.keil.com/pack/doc/CMSIS/General/html/index.html">CMSIS library</a>
  */
  void correlate(FloatArray operand2, FloatArray destination);
  
  /**
   * Correlation between arrays.
   * Sets **destination** to the correlation of *this* array and **operand2**.
   * @param[in] operand2 the second operand for the correlation
   * @param[out] destination array. It must have a minimum size of 2*max(srcALen, srcBLen)-1
   * @remarks It is the same as correlate(), but destination must have been initialized to 0 in advance. 
  */
  void correlateInitialized(FloatArray operand2, FloatArray destination);

  /**
   * Convert gains to decibel values: dB = log10(gain)*20
   * Gain 0.5 = -6dB, 1.0 = 0dB, 2.0 = +6dB
   */
  void gainToDecibel(FloatArray destination);

  /**
   * Convert decibel to gains values: gain = 10^(dB/20)
   * -6dB = 0.5, 0dB = 1.0, +6dB = 2.0
   */  
  void decibelToGain(FloatArray destination);
  
  /**
   * A subset of the array.
   * Returns a array that points to subset of the memory used by the original array.
   * @param[in] offset the first element of the subset.
   * @param[in] length the number of elments in the new FloatArray.
   * @return the newly created FloatArray.
   * @remarks no memory is allocated by this method. The memory is still shared with the original array.
   * The memory should not be de-allocated elsewhere (e.g.: by calling FloatArray::destroy() on the original FloatArray) 
   * as long as the FloatArray returned by this method is still in use.
   * @remarks Calling FloatArray::destroy() on a FloatArray instance created with this method might cause an exception.
  */
  FloatArray subArray(int offset, size_t length);
  /**
   * Create a linear ramp from one value to another.
   * Interpolates all samples in the FloatArray between the endpoints @param from to @param to.
   */
  void ramp(float from, float to);

  /**
   * Scale all values along a linear ramp from one value to another.
   */  
  void scale(float from, float to, FloatArray destination);

  /**
   * In-place scale.
   */  
  void scale(float from, float to){
    scale(from, to, *this);
  }

  /**
   * Apply tanh to each element in the array
   */
  void tanh(FloatArray destination);

  /**
   * In-place tanh
   */
  void tanh(){
    tanh(*this);
  }

  /**
   * Creates a new FloatArray.
   * Allocates size*sizeof(float) bytes of memory and returns a FloatArray that points to it.
   * @param size the size of the new FloatArray.
   * @return a FloatArray which **data** point to the newly allocated memory and **size** is initialized to the proper value.
   * @remarks a FloatArray created with this method has to be destroyed invoking the FloatArray::destroy() method.
  */
  static FloatArray create(int size);
  
  /**
   * Destroys a FloatArray created with the create() method.
   * @param array the FloatArray to be destroyed.
   * @remarks the FloatArray object passed as an argument should not be used again after invoking this method.
   * @remarks a FloatArray object that has not been created by the FloatArray::create() method might cause an exception if passed as an argument to this method.
  */
  static void destroy(FloatArray array);
};

#endif // __FloatArray_h__
