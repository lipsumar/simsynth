#pragma once

#include <Oscil.h>
#include <ADSR.h>
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include "../../Instrument.h"

class FMInstrument : public Instrument {
public:
    FMInstrument(MIDIToFreqFn freqConverter);
    
    void setup() override;
    void noteOn(uint8_t note, uint8_t velocity) override;
    void noteOff(uint8_t note, uint8_t velocity) override;
    void updateControl() override;
    int16_t updateAudio() override;
    void setPot1Value(float value) override;
    void setPot2Value(float value) override;
    void setPot3Value(float value) override;
    void setEnvelope(byte envNumber) override;
private:
    Oscil<SIN2048_NUM_CELLS, MOZZI_AUDIO_RATE> oscCarrier;
    Oscil<SIN2048_NUM_CELLS, MOZZI_AUDIO_RATE> oscModulator1;
    Oscil<SAW2048_NUM_CELLS, MOZZI_AUDIO_RATE> oscModulator2;
    ADSR<MOZZI_AUDIO_RATE, MOZZI_AUDIO_RATE> envelope;

    long mod_intensity;
    float mod_ratio1;
    const float mod_ratio2;  // Fixed ratio for second modulator
    
    uint8_t currentNote;
    bool isNoteOn;
};
