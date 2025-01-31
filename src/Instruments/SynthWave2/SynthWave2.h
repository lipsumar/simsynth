#pragma once

#include <Oscil.h>
#include <tables/saw2048_int8.h>
#include <ADSR.h>
#include <LowPassFilter.h>
#include "../../Instrument.h"

class SynthwaveSynth : public Instrument {
public:
    SynthwaveSynth(MIDIToFreqFn freqConverter);
    void setup() override;
    void noteOn(uint8_t note, uint8_t velocity) override;
    void noteOff(uint8_t note, uint8_t velocity) override;
    void updateControl() override;
    int16_t updateAudio() override;
    void setPot1Value(float value) override;
    void setPot2Value(float value) override;
    void setPot3Value(float value) override;
    void setEnvelope(uint8_t envNumber) override;

private:
    // Multiple saw oscillators for thick sound
    Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> osc1;
    Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> osc2;
    Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> osc3;
    
    // Envelope for amplitude
    ADSR<CONTROL_RATE, AUDIO_RATE> ampEnvelope;
    // Separate envelope for filter
    ADSR<CONTROL_RATE, AUDIO_RATE> filterEnvelope;
    
    LowPassFilter lpf;

    float baseFreq;
    float detune;         // Detune amount for oscillators
    int filterCutoff;     // Base filter cutoff
    float filterEnvAmount; // Amount of filter envelope modulation
    float filterResonance;
};