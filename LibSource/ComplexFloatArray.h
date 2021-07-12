#ifndef __ComplexFloatArray_h__
#define __ComplexFloatArray_h__

#include "FloatArray.h"
#include "basicmaths.h"
/**
* A structure defining a floating point complex number as two members of type float.
*/
struct ComplexFloat {
  constexpr ComplexFloat() : re(0), im(0) {}
  constexpr ComplexFloat(float x) : re(x), im(0) {}
  constexpr ComplexFloat(float re, float im) : re(re), im(im) {}

  /**
   * The real part of the complex number.
   */
  float re;
  
  /**
   * The imaginary part of the complex number.
   */
  float im;
  
  /**
   * Get the magnitude of the complex number.
   * Computes and returns the magnitude of the complex number.
   * @return The magnitude of the complex number.
   */
  float getMagnitude() const{
    return sqrtf(re*re+im*im);  
  }
  
  /**
   * Get the squared magnitude of the complex number.
   * Computes and returns the squaredmagnitude of the complex number,
   * which is cheaper to compute than the magnitude itself.
   * @return The squared magnitude of the complex number.
   */
  float getMagnitudeSquared() const{
    return re*re+im*im;  
  }
  
  /**
   * Get the phase of the complex number.
  * Computes and returns the phase of the complex number.
  * @return The phase of the complex number.
  */  
  float getPhase() const{
    return atan2f(im,re);
  }
  
  /**
     * Set the phase of the complex number.
  * Set the phase of the complex number, keeping the magnitude unaltered.
  * @param phase The new phase of the complex number
  */
  void setPhase(float phase){
    float magnitude = getMagnitude();
    setPolar(magnitude, phase);
  }
  
  /**
  * Set the magnitude of the complex number.
  * Set the magnitude of the complex number, keeping the phase unaltered.
  * @param magnitude The new magnitude of the complex number
  */
  void setMagnitude(float magnitude){
    float phase = getPhase();
    setPolar(magnitude, phase);
  }  
  
  /**
  * Set magnitude and phase of the complex number.
  * @param magnitude The new magnitude of the complex number
  * @param phase The new phase of the complex number
  */
  void setPolar(float magnitude, float phase){
    re = magnitude*cosf(phase);
    im = magnitude*sinf(phase);
  }

  /**
   * Returns complet conjugate - a copy of current number with imaginary part inverted
   */
  ComplexFloat getComplexConjugate() const {
    return ComplexFloat {re, -im};
  }


  /**
   * Returns dot product with another complex float value
   */
  ComplexFloat getDotProduct(ComplexFloat other) const {
    return ComplexFloat {re * other.re - im  * other.im, re * other.im + im * other.re};
  }

  bool operator<(const ComplexFloat& other) const {
    return getMagnitudeSquared() < other.getMagnitudeSquared();
  }

  bool operator>(const ComplexFloat& other) const {
    return getMagnitudeSquared() > other.getMagnitudeSquared();
  }

  bool operator<=(const ComplexFloat& other) const {
    return getMagnitudeSquared() <= other.getMagnitudeSquared();
  }

  bool operator>=(const ComplexFloat& other) const {
    return getMagnitudeSquared() >= other.getMagnitudeSquared();
  }

  bool operator==(const ComplexFloat& other) const {
    return re == other.re && im == other.im;
  }
  
  bool operator!=(const ComplexFloat& other) const {
    return re != other.re || im != other.im;
  }

  friend const ComplexFloat operator+(const ComplexFloat&lhs, const ComplexFloat& rhs) {
    ComplexFloat result = lhs;
    result += rhs;
    return result;
  }

  friend const ComplexFloat operator+(const ComplexFloat&lhs, float rhs) {
    ComplexFloat result = lhs;
    result += rhs;
    return result;
  }

  ComplexFloat& operator+=(float other) {
    re += other;
    im += other;
    return *this;
  }

  ComplexFloat& operator+=(const ComplexFloat& other) {
    re += other.re;
    im += other.im;
    return *this;
  }

  friend const ComplexFloat operator-(const ComplexFloat&lhs, const ComplexFloat& rhs) {
    ComplexFloat result = lhs;
    result -= rhs;
    return result;
  }

  friend const ComplexFloat operator-(const ComplexFloat&lhs, float rhs) {
    ComplexFloat result = lhs;
    result -= rhs;
    return result;
  }

  ComplexFloat& operator-=(float other) {
    re -= other;
    im -= other;
    return *this;
  }

  ComplexFloat& operator-=(const ComplexFloat& other) {
    re -= other.re;
    im -= other.im;
    return *this;
  }

  friend const ComplexFloat operator*(const ComplexFloat&lhs, const ComplexFloat& rhs) {
    ComplexFloat result = lhs;
    result *= rhs;
    return result;
  }

  friend const ComplexFloat operator*(const ComplexFloat&lhs, float rhs) {
    ComplexFloat result = lhs;
    result *= rhs;
    return result;
  }

  ComplexFloat& operator*=(float other) {
    re *= other;
    im *= other;
    return *this;
  }

  ComplexFloat& operator*=(const ComplexFloat& other) {
    re = re * other.re - im * other.im;
    im = re * other.im + im * other.re;
    return *this;
  }

  friend const ComplexFloat operator/(const ComplexFloat&lhs, float rhs) {
    ComplexFloat result = lhs;
    result /= rhs;
    return result;
  }

  ComplexFloat& operator/=(float other) {
    re /= other;
    im /= other;
    return *this;
  }

};

class ComplexFloatArray : public SimpleArray<ComplexFloat> {
public:
  ComplexFloatArray(){}
  ComplexFloatArray(ComplexFloat* data, size_t size) :
    SimpleArray(data, size) {}
    
  /** 
   * Get the real part of an element of the array.      
   * @param i The index of the element
   * @return The real part of the element
   */
  float re(const int i){
    return data[i].re;
  }
  
  /**
   *  Get the imaginary part of an element of the array.
   * @param i The index of the element
   * @return The imaginary part of the element
   */
  float im(const int i){
    return data[i].im;
  }
  
  void clear(){
    setAll(0);
  }

  /**
   * Get the magnitude of an element of the array.
   *  @param i The index of the element
   *  @return The magnitude of the element
   */  
  float mag(const int i);
  
  /**
   * The magnitudes of the elements of the array.
   * @param[out] destination The array where the magnitude values will be stored.
   */  
  void getMagnitudeValues(FloatArray destination);
  
  /**
   * The magnitude squared of an element of the array. Faster than mag().
   * @param i The index of the element
   * @return The magnitude squared of the element
   */  
  float mag2(const int i);
  
  /**
   * The squared magnitudes of the elements of the array. Faster than getMagnitudeValues().
   * @param[out] destination The array where the magnitude squared values will be stored.
   */  
  void getMagnitudeSquaredValues(FloatArray destination);
  
  /**
   * The complex conjugate values of the element of the array.
   * @param[out] destination The array where the complex conjugate values will be stored.
   */  
  void getComplexConjugateValues(ComplexFloatArray destination);
  
  /**
   * Complex dot product between arrays.
   * @param[in] operand2 The second operand of the dot product
   * @param[out] result The array where the result of the dot product is stored 
   */  
  void complexDotProduct(ComplexFloatArray operand2, ComplexFloat& result);

  /**
   * Complex by complex multiplication between arrays.
   * @param[in] operand2 The second operand of the multiplication
   * @param[out] result The array where the result of the multiplication is stored 
   */  
  void complexByComplexMultiplication(ComplexFloatArray operand2, ComplexFloatArray result);
  
  /**
   * Complex by real multiplication between arrays.
   * @param[in] operand2 The second operand of the multiplication
   * @param[out] result The array where the result of the multiplication is stored 
   */ 
  void complexByRealMultiplication(FloatArray operand2, ComplexFloatArray result);

  /**
   * Element-wise sum between complex arrays.
   * Sets each element in **destination** to the complex sum of the corresponding element of the array and **operand2**
   * @param[in] operand2 second operand for the sum
   * @param[out] destination the destination array
  */
  void add(ComplexFloatArray operand2, ComplexFloatArray destination);

  /**
   * In-place element-wise sum between complex arrays.
   * Adds each element of **operand2** to the corresponding element in the array.
   * @param operand2 second operand for the sum
  */
  void add(ComplexFloatArray operand2);

  /**
   * Element-wise difference between complex arrays.
   * Sets each element in **destination** to the complex difference between the corresponding element of the array and **operand2**
   * @param[in] operand2 second operand for the subtraction
   * @param[out] destination the destination array
  */
  void subtract(ComplexFloatArray operand2, ComplexFloatArray destination);

  /**
   * In-place element-wise difference between complex arrays.
   * Sutracts each element of **operand2** from the corresponding element in the array.
   * @param operand2 second operand for the sum
  */
  void subtract(ComplexFloatArray operand2);
  
  /**
    The value of the element with the maximum magnitude in the array.
    @return The maximum magnitude value in the array.
  */ 
  float getMaxMagnitudeValue();
  
  /**
    The index of the element with the maximum magnitude in the array.
    @return The index of the element with the maximum magnitude in the array.
  */ 
  int getMaxMagnitudeIndex();
  
  /**
   * A subset of the array.
   * Returns an array that points to subset of the memory used by the original array.
   * @param[in] offset the first element of the subset.
   * @param[in] length the number of elments in the new ComplexFloatArray.
   * @return the newly created ComplexFloatArray.
   * @remarks no memory is allocated by this method. The memory is still shared with the original array.
   * The memory should not be de-allocated elsewhere (e.g.: by calling ComplexFloatArray::destroy() on the original ComplexFloatArray) 
   * as long as the ComplexFloatArray returned by this method is still in use.
   * @remarks Calling ComplexFloatArray::destroy() on a ComplexFloatArray instance created with this method might cause an exception.
  */
  ComplexFloatArray subArray(int offset, size_t length);
  
  /** Get the real part of the elements of the array.
   * @param[out] buf The array where the real part will be stored.
   */   
  void getRealValues(FloatArray buf);
  
  /** Get the imaginary part of the elements of the array.
   * @param[out] buf The array where the imaginary part will be stored.
  */   
  void getImaginaryValues(FloatArray buf);
  
  /** Array by scalar multiplication.
   * @param factor The value by which all the elements of the array are multiplied.
   */
  void scale(float factor);
  
  /**
   * Creates a new ComplexFloatArray.
   * Allocates size*sizeof(float) bytes of memory and returns a ComplexFloatArray that points to it.
   * @param size The size of the new ComplexFloatArray.
   * @return A ComplexFloatArray which **data** point to the newly allocated memory and <code>size</code> is initialized to the proper value.
   * @remarks A ComplexFloatArray created with this method has to be destroyed invoking the ComplexFloatArray::destroy() method.
  */
  static ComplexFloatArray create(size_t size);

  /**
   * Destroys a ComplexFloatArray created with the create() method.
   * @param array The ComplexFloatArray to be destroyed.
   * @remarks The ComplexFloatArray object passed as an argument should not be used again after invoking this method.
   * @remarks A ComplexFloatArray object that has not been created by the ComplexFloatArray::create() method might cause an exception if passed as an argument to this method.
  */
  static void destroy(ComplexFloatArray);

  /**
   * Copies real values from a FloatArray, sets imaginary values to 0
   * @param[in] source The source array
  */
  void fromFloat(FloatArray source);
  
  /**
   * Copies real and imaginary values of the ComplexFloatArray into a FloatArray.
   * @param[out] destination The destination array
   * @remarks The size of the FloatArray must be twice the size of the ComplexFloatArray.
  */
  void toFloat(FloatArray destination);

  /**
   * Set all the elements in the array.
   * @param[in] value All the elements are set to this value.
  */
  void setAll(ComplexFloat value);
  
  /**
   * Set all the elements in the array.
   * @param[in] value The real and imaginary parts of all the elements in the array are set to this value.
  */
  void setAll(float value);
  
  /**
   * Set all the elements in the array.
   * @param[in] valueRe The real part of every element of the the array will be set to this value.
   * @param[in] valueIm The imaginary pary of rvery element of the the array will be set to this value.
  */
  void setAll(float valueRe, float valueIm);

  /**
   * Get polar coordinates for all the elements in the array.
   * @param[out] magnitude An array containing the magnitudes.
   * @param[out] phase An array containing the phases.
  */
  void getPolar(FloatArray magnitude, FloatArray phase);

  /**
   * Set all the elements in the array using polar coordinates.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[in] phase An array containing the phases.
  */
  void setPolar(FloatArray magnitude, FloatArray phase);
  
  /**
   * Set a range of elements in the array using polar coordinates.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[in] phase An array containing the phases.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
  */  
  void setPolar(FloatArray magnitude, FloatArray phase, int offset, size_t count);
    
  /**
   * The phases of the elements of the array.
   * @param[out] destination The array where the phase values will be stored.
   */  
  void getPhaseValues(FloatArray destination);

  /**
   * Set the phase of the elements of the array, leaving the magnitude unchanged.
   * @param[in] phase An array containing the phases.
  */
  void setPhase(FloatArray phase);

  /**
   * Set the phase of a range of elements of the array, leaving the magnitude unchanged.
   * @param[in] phase An array containing the phases.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
  */
  void setPhase(FloatArray phase, int offset, size_t count);
  
  /**
   * Set the phase of the elements of an array, using the magnitude from the current array.
   * @param[in] phase An array containing the phases.
   * @param[out] destination The destination array.
  */
  void setPhase(FloatArray phase, ComplexFloatArray destination);
  
  /**
   * Set the phase of a range of the elements of an array, using the magnitude from the current array.
   * 
   * The <code>offset</code> and <code>count</code> specified apply to both the source and <code>destination</code> arrays. 
   * Elements of the <code>destination</code> array outside this range will not be affected.
   * @param[in] phase An array containing the phases.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
   * @param[out] destination The destination array.
  */
  void setPhase(FloatArray phase, int offset, size_t count, ComplexFloatArray destination);


  /**
   * Set the magnitude of the elements of the array, leaving the phase unchanged.
   * @param[in] magnitude An array containing the magnitudes.
  */
  void setMagnitude(FloatArray magnitude);

/**
   * Set the magnitude of a range of elements of the array, leaving the phase unchanged.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
  */
  void setMagnitude(FloatArray magnitude, int offset, size_t count);
  
  /**
   * Set the magnitude of the elements of an array, using the phase from the current array.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[out] destination The destination array.
  */
  void setMagnitude(FloatArray magnitude, ComplexFloatArray destination);
  
  /**
   * Set the magnitude of a range of the elements of an array, using the phases from the current array.
   * 
   * The <code>offset</code> and <code>count</code> specified apply to both the source and <code>destination</code> arrays. 
   * Elements of the <code>destination</code> array outside this range will not be affected.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
   * @param[out] destination The destination array.
  */
  void setMagnitude(FloatArray magnitude, int offset, size_t count, ComplexFloatArray destination);
};

#endif // __ComplexFloatArray_h__
