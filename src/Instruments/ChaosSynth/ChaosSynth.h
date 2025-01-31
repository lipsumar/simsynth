#pragma once

#include <Oscil.h>
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/triangle2048_int8.h>
#include <ADSR.h>
#include <LowPassFilter.h>
#include "../../Instrument.h"

class ChaosSynth : public Instrument {
public:
    ChaosSynth(MIDIToFreqFn freqConverter);
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
    Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> mainOsc;
    Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> ringMod;
    Oscil<TRIANGLE2048_NUM_CELLS, CONTROL_RATE> lfoFilter;
    
    ADSR<CONTROL_RATE, AUDIO_RATE> envelope;
    LowPassFilter lpf;

    float baseFreq;
    int filterCutoff;
    float ringModAmount;
    float filterLfoAmount;
    float currentLfoValue;
};

