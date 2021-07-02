#ifndef _Interpolator_h_
#define _Interpolator_h_

#include <stdint.h>

class Interpolator {
public:
  static float linear(float y1, float y2, float mu){
    return y1 + (y2 - y1) * mu;
  }
  static float cosine(float y1, float y2, float mu){
    float mu2 = (1-cosf(mu*M_PI))/2;
    return y1*(1-mu2)+y2*mu2;
  }
  /** 
   * Three-point cubic interpolation of point between y1 and y2
   * ref: http://www.ebyte.it/library/codesnippets/P3Interpolation.html
   */
  static float cubic(float y0, float y1, float y2, float mu){
    float d1 = 0.5*(y2-y0);
    float d2 = y2-y1+y0-y1;
    float dx = mu;
    return y1 + dx*(d1+0.5*d2*dx);
  }
  /**
   * Four-point cubic interpolation
   * Get the interpolated value of a curve described by y0 to y4,
   * somewhere between the points y1 and y2.
   */
  static float cubic(float y0, float y1, float y2, float y3, float mu) {
    float mu2 = mu*mu;
    float a0 = y3 - y2 - y0 + y1;
    float a1 = y0 - y1 - a0;
    float a2 = y2 - y0;
    float a3 = y1;
    return a0*mu*mu2+a1*mu2+a2*mu+a3;
  }
  static float cubicSmooth(float y0, float y1, float y2, float y3, float mu) {
    // use Catmull-Rom splines:
    // take the slope between the previous point and the next as the derivative at the current point
    float mu2 = mu*mu;
    float a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
    float a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
    float a2 = -0.5*y0 + 0.5*y2;
    float a3 = y1;
    return a0*mu*mu2+a1*mu2+a2*mu+a3;
  }
  /*
    Tension: 1 is high, 0 normal, -1 is low
    Bias: 0 is even,
         positive is towards first segment,
         negative towards the other
  */
  static float hermite(float y0, float y1, float y2, float y3, float mu, float tension=0, float bias=0){
    float mu2 = mu * mu;
    float mu3 = mu2 * mu;
    float m0  = (y1-y0)*(1+bias)*(1-tension)/2;
    m0 += (y2-y1)*(1-bias)*(1-tension)/2;
    float m1  = (y2-y1)*(1+bias)*(1-tension)/2;
    m1 += (y3-y2)*(1-bias)*(1-tension)/2;
    float a0 =  2*mu3 - 3*mu2 + 1;
    float a1 =    mu3 - 2*mu2 + mu;
    float a2 =    mu3 -   mu2;
    float a3 = -2*mu3 + 3*mu2;
   return(a0*y1+a1*m0+a2*m1+a3*y2);
  }
};

enum InterpolationMethod {
  LINEAR_INTERPOLATION,
  COSINE_INTERPOLATION,
  CUBIC_3P_INTERPOLATION,
  CUBIC_4P_INTERPOLATION,
  CUBIC_4P_SMOOTH_INTERPOLATION,
  HERMITE_INTERPOLATION
};

#endif /* _Interpolator_h_ */
