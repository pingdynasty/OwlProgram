#include "TestPatch.hpp"
#include "QuadratureSineOscillator.h"

class QuadratureSineOscillatorTestPatch : public TestPatch {
public:
  QuadratureSineOscillatorTestPatch(){
    {
      TEST("ctor");
      QuadratureSineOscillator osc;
      osc.setSampleRate(16000);
      osc.setFrequency(440);
      CHECK_EQUAL(osc.getSampleRate(), 16000);
      CHECK_EQUAL((int)osc.getFrequency(), 440);
      osc = QuadratureSineOscillator();
      osc.setSampleRate(32000);
      CHECK_EQUAL(osc.getFrequency(), 0.0f);
      CHECK_EQUAL(osc.getSampleRate(), 32000);
    }
    {
      TEST("create");
      QuadratureSineOscillator* osc = QuadratureSineOscillator::create(32000);
      osc->setFrequency(120);
      CHECK_EQUAL(osc->getSampleRate(), 32000);
      CHECK_EQUAL(osc->getFrequency(), 120);
      QuadratureSineOscillator::destroy(osc);
    }
    {
      TEST("setSampleRate");
      QuadratureSineOscillator* osc = QuadratureSineOscillator::create(44100);
      osc->setFrequency(321);
      CHECK_EQUAL(osc->getSampleRate(), 44100);
      CHECK_EQUAL(osc->getFrequency(), 321);
      osc->setSampleRate(48000);
      CHECK_EQUAL((int)osc->getSampleRate(), 48000);
      CHECK_EQUAL(osc->getFrequency(), 321); // check that frequency is the same
      QuadratureSineOscillator::destroy(osc);      
    }
    {
      TEST("compareBlockAndSampleBased");
      QuadratureSineOscillator* osc1 = QuadratureSineOscillator::create(48000);
      QuadratureSineOscillator* osc2 = QuadratureSineOscillator::create(48000);
      osc1->setFrequency(480);      
      osc2->setFrequency(480);      
      AudioBuffer* s1 = AudioBuffer::create(2, 1000);
      AudioBuffer* s2 = AudioBuffer::create(2, 1000);
      osc1->generate(*s1);
      for(size_t i=0; i<1000; ++i) {
        ComplexFloat sample = osc2->generate();
        s2->getSamples(0)[i] = sample.re;
        s2->getSamples(1)[i] = sample.im;
      }
      for(size_t i=0; i<1000; ++i) {
        CHECK_CLOSE(s1->getSamples(0)[i], s2->getSamples(0)[i], 0.00002);
        CHECK_CLOSE(s1->getSamples(1)[i], s2->getSamples(1)[i], 0.00002);
      }
      osc1->generate(*s1);
      for(size_t i=0; i<1000; ++i) {
        ComplexFloat sample = osc2->generate();
        s2->getSamples(0)[i] = sample.re;
        s2->getSamples(1)[i] = sample.im;
      }
      for(size_t i=0; i<1000; ++i) {
        CHECK_CLOSE(s1->getSamples(0)[i], s2->getSamples(0)[i], 0.00002);
        CHECK_CLOSE(s1->getSamples(1)[i], s2->getSamples(1)[i], 0.00002);
      }
      AudioBuffer::destroy(s1);      
      AudioBuffer::destroy(s2);      
      QuadratureSineOscillator::destroy(osc1);      
      QuadratureSineOscillator::destroy(osc2);      
    }
}
};
