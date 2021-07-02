#include "TestPatch.hpp"
#include "SineOscillator.h"

class SineOscillatorTestPatch : public TestPatch {
public:
  SineOscillatorTestPatch(){
    {
      TEST("ctor");
      SineOscillator osc;
      osc.setSampleRate(16000);
      osc.setFrequency(440);
      CHECK_EQUAL(osc.getSampleRate(), 16000);
      CHECK_EQUAL((int)osc.getFrequency(), 440);
      osc = SineOscillator();
      osc.setSampleRate(32000);
      CHECK_EQUAL(osc.getFrequency(), 0.0f);
      CHECK_EQUAL(osc.getSampleRate(), 32000);
    }
    {
      TEST("create");
      SineOscillator* osc = SineOscillator::create(32000);
      osc->setFrequency(120);
      CHECK_EQUAL(osc->getSampleRate(), 32000);
      CHECK_EQUAL(osc->getFrequency(), 120);
      SineOscillator::destroy(osc);
    }
    {
      TEST("setSampleRate");
      SineOscillator* osc = SineOscillator::create(44100);
      osc->setFrequency(321);
      CHECK_EQUAL(osc->getSampleRate(), 44100);
      CHECK_EQUAL(osc->getFrequency(), 321);
      osc->setSampleRate(48000);
      CHECK_EQUAL((int)osc->getSampleRate(), 48000);
      CHECK_EQUAL(osc->getFrequency(), 321); // check that frequency is the same
      SineOscillator::destroy(osc);      
    }
    {
      TEST("compareBlockAndSampleBased");
      SineOscillator* osc1 = SineOscillator::create(48000);
      SineOscillator* osc2 = SineOscillator::create(48000);
      osc1->setFrequency(480);      
      osc2->setFrequency(480);      
      FloatArray s1 = FloatArray::create(1000);
      FloatArray s2 = FloatArray::create(1000);
      osc1->generate(s1);
      for(size_t i=0; i<1000; ++i)
	s2[i] = osc2->generate();
      for(size_t i=0; i<1000; ++i)
	CHECK_CLOSE(s1[i], s2[i], 0.00002);
      osc1->generate(s1);
      for(size_t i=0; i<1000; ++i)
	s2[i] = osc2->generate();
      for(size_t i=0; i<1000; ++i)
	CHECK_CLOSE(s1[i], s2[i], 0.00002);
      FloatArray::destroy(s1);      
      FloatArray::destroy(s2);      
      SineOscillator::destroy(osc1);      
      SineOscillator::destroy(osc2);      
    }
}
};

