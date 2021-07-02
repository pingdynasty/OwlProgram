#include "WavetableOscillator.h"
#include <stdint.h>

WavetableOscillator::WavetableOscillator(float sr, const FloatArray wavetable):
  mul(1.0f/sr),
  wave(wavetable),
  acc(0.0f), inc(0.1f)
{}
 
void WavetableOscillator::setSampleRate(float sr){
  mul = 1.0f/sr;
}

float WavetableOscillator::getSampleRate(){
  return 1.0f/mul;
}

void WavetableOscillator::setFrequency(float freq){
  inc = freq*mul;
}

float WavetableOscillator::getFrequency(){
  return inc/mul;
}

float WavetableOscillator::getSample(float phase){
  uint32_t size = wave.getSize();
  uint32_t index = phase*(size-1);
  index = min(index, size-1);
  return wave[index];
}

void WavetableOscillator::setPhase(float phase){
  acc = phase/(2*M_PI);
}

float WavetableOscillator::getPhase(){
  return acc*2*M_PI;
}

float WavetableOscillator::generate(){
  float s = getSample(acc);
  acc += inc;
  if(acc >= 1.0f)
    acc -= 1.0f;
  return s;
}

float WavetableOscillator::generate(float fm){
  float s = getSample(acc);
  acc += inc * (1 + fm);
  if(acc > 1.0f)
    acc -= 1.0f;
  return s;
}

void WavetableOscillator::generate(FloatArray samples){
  for(size_t i=0; i<samples.getSize();++i)
    samples[i] = generate();
}

void WavetableOscillator::generate(FloatArray output, FloatArray fm){
  for(size_t i=0; i<output.getSize(); ++i){
    output[i] = getSample(acc);
    acc += inc * (1 + fm[i]);
    if(acc > 1.0f)
      acc -= 1.0f;
  }
}

WavetableOscillator* WavetableOscillator::create(float sr, size_t size) {
  FloatArray wave = FloatArray::create(size);
  for(size_t i=0; i<size; ++i)
    wave[i] = sinf(2*M_PI*i/(size-1));    
  return new WavetableOscillator(sr, wave);
}

void WavetableOscillator::destroy(WavetableOscillator* osc){
  FloatArray::destroy(osc->wave);
  delete osc;
}
