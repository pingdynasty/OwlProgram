#include "TestPatch.hpp"
#include "ShortArray.h"

class ShortArrayTestPatch : public TestPatch {
public:
  ShortArrayTestPatch(){
    {
      TEST("Default ctor");
      ShortArray empty;
      CHECK_EQUAL(empty.getSize(), 0);
      // CHECK_EQUAL((short*)empty, (short*)NULL);
      CHECK(empty.getData() == NULL);
    }
    {
      TEST("create");
      ShortArray array = ShortArray::create(512);
      CHECK_EQUAL(array.getSize(), 512);
      REQUIRE(array.getData() != NULL);
      for(int i=0; i<512; ++i){
	CHECK_CLOSE(array[i], 0.0, DEFAULT_TOLERANCE);
      }
    }
    {
      TEST("getSize");
      int size = 123;
      ShortArray array = ShortArray::create(size);
      CHECK_EQUAL(size, array.getSize());
    }
    {
      TEST("minmax");
      ShortArray ar = ShortArray::create(10);
      ar[0] = 0;
      ar[1] = 1;
      ar[2] = 2;
      ar[3] = -1;
      ar[4] = -2;
      ar[5] = -3;
      ar[6] = 3;
      ar[7] = 4;
      ar[8] = 2;
      ar[9] = -2;
      int16_t value;
      int index;
      ar.getMin(&value, &index);
      CHECK(value == -3);
      CHECK(index == 5);
      CHECK(value == ar.getMinValue());
      CHECK(index == ar.getMinIndex());

      ar.getMax(&value, &index);
      CHECK(value == 4);
      CHECK(index == 7);
      CHECK(value == ar.getMaxValue());
      CHECK(index == ar.getMaxIndex());
    }
    {
      TEST("clear,noise");
      ShortArray ar = ShortArray::create(1000);
      int32_t acc = 0;
      for(int n = 0; n < ar.getSize(); ++n){
        acc += abs(ar[n]);
      }
      // A newly created arra should be init'd to 0
      CHECK(acc==0);
      acc = 0;
      ar.noise();
      for(int n = 0; n < ar.getSize(); ++n){
        acc += abs(ar[n]);
      }
      CHECK(acc > 0);
      ar.clear();
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK(ar[n] == 0);
      }
      ar.noise();
      CHECK(ar.getMinValue() < -200);
      CHECK(ar.getMaxValue() > 200);
    }
    {
      TEST("copy,equal");
      ShortArray ar = ShortArray::create(100);
      ShortArray ar2 = ShortArray::create(100);
      ar.noise();
      CHECK(ar.equals(ar));
      ar.copyTo(ar2);
      CHECK(ar2.equals(ar));
      ar.clear();
      ar.copyFrom(ar2);
      CHECK(ar.equals(ar));
    }
    {
      TEST("rectify");
      ShortArray ar = ShortArray::create(1000);
      ShortArray ar2 = ShortArray::create(1000);
      ar.noise();
      ar2.copyFrom(ar);
      ar2.rectify();
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK(abs(ar[n]) == ar2[n]); 
      }
      ar2.clear();
      ar.rectify(ar2);
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK(abs(ar[n]) == ar2[n]); 
      }
    }
    {
      TEST("reverse");
      ShortArray ar = ShortArray::create(1000);
      ShortArray ar2 = ShortArray::create(1000);
      ar.noise();
      ar.copyTo(ar2);
      ar2.reverse();
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK(ar[n] == ar2[ar2.getSize() - n - 1]); 
      }
      ar.reverse(ar2);
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK(ar[n] == ar2[ar2.getSize() - n - 1]); 
      }
    }
    {
      TEST("reciprocal");
      ShortArray ar = ShortArray::create(1000);
      ShortArray ar2 = ShortArray::create(1000);
      ar.noise();
      ar.copyTo(ar2);
      ar2.reciprocal();
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK_CLOSE(ar2[n], (0.5 + 1.f/ar[n]), 2);
      }
      ar.reciprocal(ar2);
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK_CLOSE(ar2[n], (0.5 + 1.f/ar[n]), 2);
      }
    }
    {
      TEST("negate");
      ShortArray ar = ShortArray::create(1000);
      ShortArray ar2 = ShortArray::create(1000);
      ar.noise();
      ar2.copyFrom(ar);
      ar2.negate();
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK(ar2[n] == -ar[n]);
      }
      ar.negate(ar2);
      for(int n = 0; n < ar.getSize(); ++n){
        CHECK(ar2[n] == -ar[n]);
      }
        CHECK(ar[0] == 534);
        CHECK_EQUAL(ar[0], ar[44]);
        REQUIRE(ar[5] == 534);
    }
    {
      TEST("rms");
      ShortArray ar = ShortArray::create(1000);
      ShortArray ar2 = ShortArray::create(1000);
      ar.noise();
      ar.reciprocal(ar2);
      float acc = 0;
      for(int n=0; n<ar.getSize(); ++n){
        acc += ar[n] * ar[n] / (float)ar.getSize(); 
      } 
      int rms=(sqrtf(acc + 0.5));
      CHECK_CLOSE(rms, ar.getRms(), 5);
    }
   //TODO: test everything else
  }
};
