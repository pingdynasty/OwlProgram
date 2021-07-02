#include "PolyBlepOscillator.h"
#include "basicmaths.h"

PolyBlepOscillator::PolyBlepOscillator(float sr):
  mul(1.0f/sr),
  nfreq(0.0f), 
  shape(0.5f), 
  pw(0.5f) {
  osc.Init();
}

PolyBlepOscillator::PolyBlepOscillator(float freq, float sr):
  mul(1.0f/sr),
  shape(0.5f), 
  pw(0.5f) {
  setFrequency(freq);
  osc.Init();
}

void PolyBlepOscillator::setSampleRate(float sr){
  mul = 1.0f/sr;
}

float PolyBlepOscillator::getSampleRate(){
  return 1.0f/mul;
}

void PolyBlepOscillator::setFrequency(float freq){
  nfreq = mul*freq;
}

float PolyBlepOscillator::getFrequency(){
  return nfreq/mul;
}

void PolyBlepOscillator::setShape(float value){
  shape = value;
}

float PolyBlepOscillator::getShape(){
  return shape;
}

void PolyBlepOscillator::setPulseWidth(float value){
  pw = value;
}

float PolyBlepOscillator::getPulseWidth(){
  return pw;
}

float PolyBlepOscillator::getPhase(){
  return osc.getMasterPhase()*2*M_PI;
}

void PolyBlepOscillator::setPhase(float phase){
  osc.setMasterPhase(phase/(2*M_PI));
}

float PolyBlepOscillator::generate(){
  float sample;
  osc.Render<true>(nfreq, pw, shape, &sample, 1);
  return sample;
}

float PolyBlepOscillator::generate(float fm){
  float sample;
  osc.Render<true>(nfreq*(1+fm), pw, shape, &sample, 1);
  return sample;
}

void PolyBlepOscillator::generate(FloatArray output){
  osc.Render<true>(nfreq, pw, shape, output, output.getSize());
}

void PolyBlepOscillator::generate(FloatArray output, FloatArray fm){
  output.copyFrom(fm);
  output.multiply(nfreq); // scale by base frequency
  output.add(nfreq); // add base frequency : freq = nfreq + nfreq*fm
  osc.Render<true>(output, pw, shape, output, output.getSize());
}

void PolyBlepOscillator::getSamples(FloatArray output, FloatArray freqs){
  freqs.multiply(mul); // normalise frequencies
  osc.Render<true>(freqs, pw, shape, output, output.getSize());
}

PolyBlepOscillator* PolyBlepOscillator::create(float sr){
  return new PolyBlepOscillator(sr);
}

PolyBlepOscillator* PolyBlepOscillator::create(float freq, float sr){
  return new PolyBlepOscillator(freq, sr);
}

void PolyBlepOscillator::destroy(PolyBlepOscillator* osc){
  delete osc;
}
