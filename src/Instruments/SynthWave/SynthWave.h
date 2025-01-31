#pragma once

#include <stdint.h> 
#include "../../Instrument.h"
#include <Oscil.h>
#include <ADSR.h>
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <LowPassFilter.h>



class SynthWave : public Instrument {
public:
    SynthWave(MIDIToFreqFn freqConverter);
    void setup() override;
    void noteOn(byte note, byte velocity) override;
    void noteOff(byte note, byte velocity) override;
    void updateControl() override;
    int16_t updateAudio() override;
    
    // Implementation of generic pot controls
    void setPot1Value(float value) override;  // Controls filter cutoff
    void setPot2Value(float value) override;  // Controls sub oscillator mix
    void setPot3Value(float value) override; 
    void setEnvelope(byte envNumber) override;
private:
    // Oscillators
    Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> mainOsc;
    Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> mainOsc2;
    Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> subOsc;

    // Filter and envelope
    LowPassFilter lpf;
    ADSR<MOZZI_AUDIO_RATE, MOZZI_AUDIO_RATE> envelope;

    // Parameters
    float frequency;
    int scaledCutoff;
    const byte resonanceAmount;
    float subMixAmount;
};