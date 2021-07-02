////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 
 
 LICENSE:
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */


/* created by the OWL team 2013 */


////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __EnvelopeTestPatch_hpp__
#define __EnvelopeTestPatch_hpp__

#include "Patch.h"
#include "LinearAdsrEnvelope.h"

class EnvelopeTestPatch : public Patch {
public:
  LinearAdsrEnvelope env;
  FloatArray envBuffer;
  EnvelopeTestPatch():
    env(getSampleRate())
  {
    registerParameter(PARAMETER_A, "Attack");
    registerParameter(PARAMETER_B, "Decay");
    registerParameter(PARAMETER_C, "Sustain");
    registerParameter(PARAMETER_D, "Release");
    envBuffer = FloatArray::create(getBlockSize());
    debugMessage("__");
  }
  ~EnvelopeTestPatch(){
    FloatArray::destroy(envBuffer);
  }
  void processAudio(AudioBuffer &buffer){
    env.setAttack(getParameterValue(PARAMETER_A)*4);
    env.setDecay(getParameterValue(PARAMETER_B)*4);
    env.setSustain(getParameterValue(PARAMETER_C));
    env.setRelease(getParameterValue(PARAMETER_D)*4);

    FloatArray fa=buffer.getSamples(0);
    fa.noise();
    static int lastButton = GREEN_BUTTON; //to avoid automatic triggering on startup
    int button;
    if(isButtonPressed(GREEN_BUTTON))
      button = GREEN_BUTTON;
    else 
      button = RED_BUTTON;
    
    // use next two lines to test gate. Gate will be on as long as the red light is on
    bool gate = (button == RED_BUTTON);
    env.gate(gate);
    
    // use next 7 lines to test trigger
    //  env.setRetrigger(false);
    //  bool trigger = (lastButton != button);
    //  static int count = 0;
    //  if(trigger){
      //  env.trigger();
    //  }
    //  lastButton = button;
    
    // env.generate(envBuffer);
    // fa.multiply(envBuffer);
    // or:
    env.process(fa, fa);
    fa.multiply(0.2);
  }
};

#endif // __EnvelopeTestPatch_hpp__
