v21.2
-----

* Added FloatArray::softclip()
* Added clamp(x, lo, hi) macro
* Added TapTempo
* Added DryWetProcessors
* Added FeedbackProcessors
* Added CrossFadingDelayProcessor
* Added AudioBuffer::copyFrom(), copyTo(), multiply() and add()
* Added Sample oscillator
* Added Agnesi curve oscillator
* Added MPE processor
* Added VOSIM oscillator
* Added methods for sample-rate cutoff frequency modulation in StateVariableFilter and BiquadFilter
* Added VelocityCurve, MidiProcessor, Synth and AbstractSynth
* New templates for MIDI voicing: PolyphonicProcessor and MonophonicProcessor
* Added WavFile for reading wav resources and converting to FloatArray
* Changed enum for BUTTON_A, B, C to BUTTON_1, 2, 3 et c.
* Fix FloatMatrix compilation
* FAUST Checkbutton support
* FAUST Soundfile support
* Added CircularBuffer and DelayProcessor classes
* Added SmoothingFilter and DcBlockingFilter classes
* Improved BiquadFilter and StateVariableFilter
* SimpleArray templated base class
* Updated unit tests
* Improvements to ComplexFloat/ComplexShort classes
* Envelope implements SignalProcessor and SignalGenerator
* NoiseOscillator renamed NoiseGenerator
* Oscillator inherits SignalGenerator
* Window inherits SignalProcessor
* Resampler split into Upsampler and Downsampler SignalProcessors
* ShortArray and IntArray conversion methods renamed toFloat() and fromFloat()
* New SignalGenerator and SignalProcessor abstract base classes
* Refactored makefiles
* Fixed 'test' make target
* Allow patches to be compiled and run natively

v21.1
-----

* SOUL integration
* Maximilian integration
* DaisySP integration
* Added FAUST 'key' MIDI parameter (complements 'freq', 'gain', 'gate' and 'bend')
* Added StateVariableFilter DSP class
* Fixed Bus Error regression on legacy hardware
* Automatically zero-initialise all heap allocations

v21.0
-----

* Matches release v21.0 of OpenWare
* Add support for patch resources

v12.0
-----

* extended parameters: up to 24 parameters controlled by MIDI
* incoming events: handle button presses and MIDI note on/off messages in the patch
* outgoing events: send parameter and button changes as MIDI CC and note on/off messages
* parameter defaults, ranges, smoothing, hysteresis, and non-linearity (exponential and logarithmic curves)

v11.0
-----

* new library classes: AdsrEnvelope, VoltsPerOctave
* updates to web audio processing
* sample accurate button states
* full compatibility with OWL v11 firmware
* improvements to FAUST build

v10.0
-----

OwlProgram release, including OWL Library, to coincide with release v10 of the OWL Firmware.


Prehistory
-----------

A wise old OWL sat in an oak.