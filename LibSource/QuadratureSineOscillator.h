#ifndef QUADRATURE_SINE_OSCILLATOR_H
#define QUADRATURE_SINE_OSCILLATOR_H

#include "StereoOscillator.h"

class QuadratureSineOscillator : public StereoOscillatorTemplate<QuadratureSineOscillator> {
public:
    static constexpr float begin_phase = 0;
    static constexpr float end_phase = 2 * M_PI;
    ComplexFloat getSample() {
        return ComplexFloat { cosf(phase), sinf(phase) };
    }
    void generate(AudioBuffer& output) override {
        size_t len = output.getSize();
        FloatArray left = output.getSamples(0);
        FloatArray right = output.getSamples(1);
        for (size_t i = 0; i < len; ++i) {
            left[i] = cosf(phase);
            right[i] = sinf(phase);
            phase += incr; // allow phase to overrun
        }
        phase = fmodf(phase, end_phase);
    }
    void generate(AudioBuffer& output, FloatArray fm) {
        size_t len = output.getSize();
        FloatArray left = output.getSamples(0);
        FloatArray right = output.getSamples(1);
        for (size_t i = 0; i < len; ++i) {
            left[i] = cosf(phase);
            right[i] = sinf(phase);
            phase += incr * (1 + fm[i]);
            // allow phase to overrun
        }
        phase = fmodf(phase, end_phase);
    }
    using StereoOscillatorTemplate<QuadratureSineOscillator>::generate;
};

#endif /* QUADRATURE_SINE_OSCILLATOR_H */
