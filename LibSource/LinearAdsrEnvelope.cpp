#include "LinearAdsrEnvelope.h"
#include "basicmaths.h"

/*
void EnvelopeGenerator::calculateMultiplier(double startLevel,
                                            double endLevel,
                                            unsigned long long lengthInSamples) {
    multiplier = 1.0 + (log(endLevel) - log(startLevel)) / (lengthInSamples);
}
*/

LinearAdsrEnvelope::LinearAdsrEnvelope(float sampleRate) :
  sampleRate(sampleRate),
  stage(kIdle),
  trig(kGate),
  level(0.0),
  gateState(false),
  gateTime(-1) {
  setAttack(1/sampleRate);
  setDecay(1/sampleRate);
  setSustain(1.0);
  setRelease(1/sampleRate);
  setRetrigger(false);
}

LinearAdsrEnvelope::~LinearAdsrEnvelope(){}

void LinearAdsrEnvelope::setSampleRate(float value){
  sampleRate = value;
}

void LinearAdsrEnvelope::setAttack(float value){
  attackIncrement = 1/(sampleRate*value);
}

void LinearAdsrEnvelope::setDecay(float value){
  decayIncrement = - 1/(sampleRate*value);
}

void LinearAdsrEnvelope::setRelease(float value){
  releaseIncrement = - 1/(sampleRate*value);
}

void LinearAdsrEnvelope::setSustain(float newSustain){
  sustain = newSustain;
}

void LinearAdsrEnvelope::setRetrigger(bool state){
  retrigger = state;
}

void LinearAdsrEnvelope::trigger(bool state, int delay){
  gate(state, delay);
  trig = kTrigger;
}

void LinearAdsrEnvelope::gate(bool state){
  gate(state, 0);
}

void LinearAdsrEnvelope::gate(bool state, int delay){
  if(gateState != state){
    gateTime = delay;
    gateState = state;
  }
  trig = kGate;
}

float LinearAdsrEnvelope::getLevel(){
  return level;
}

void LinearAdsrEnvelope::setLevel(float newLevel){
  level = newLevel;
}

float LinearAdsrEnvelope::generate(){
  if(gateTime == 0){
    stage = kAttack;
    if(trig == kTrigger){
      gateState = false;
    }
  }
  if(gateTime >= 0){
    gateTime--; // this will stop at -1
  }
  switch (stage) {
  case kAttack:
    // attack ramp
    level += attackIncrement;
    if(level >= 1.0){
      level = 1.0;
      stage = kDecay;
    }else if(gateState == false && trig == kGate){
      stage = kRelease;
    }
    break;
  case kDecay:
    // decay ramp
    level += decayIncrement;
    if(level <= sustain){
      level = sustain;
      if(trig == kGate){
        stage = kSustain;
      } else { // (trig == kTrigger)
        stage = kRelease;
      }
    } else if(gateState == false && trig == kGate){
      stage = kRelease;
    }
    break;
  case kSustain:
    level = sustain;
    if(gateState == false){
      stage = kRelease;
    }
    break;
  case kRelease:
    // release ramp
    level += releaseIncrement;
    if(level <= 0.0){
      level = 0.0;
      if (retrigger == true)
        trigger();
      else // (retrigger == false)
        stage = kIdle;
    }else if(gateState == true ){ // if during release the gate is on again, start over from the current level
      stage = kAttack;
    }
    break;
  case kIdle:
    level = 0.0;
    if(gateState == true)
      stage = kAttack;
    break;
  }
  return level;
}
