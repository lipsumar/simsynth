#pragma once


#include <Oscil.h>
#include <ADSR.h>
#include <tables/sin2048_int8.h>
#include "../../Instrument.h"

class GentleSine : public Instrument {
public:
    GentleSine(MIDIToFreqFn freqConverter);
    virtual void setup() override;
    virtual void noteOn(uint8_t note, uint8_t velocity) override;
    virtual void noteOff(uint8_t note, uint8_t velocity) override;
    virtual void updateControl() override;
    virtual int16_t updateAudio() override;
    virtual void setPot1Value(float value) override;
    virtual void setPot2Value(float value) override;
    virtual void setPot3Value(float value) override;
    virtual void setEnvelope(uint8_t envNumber) override;

private:
    Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> mainOsc;
    Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> detuneOsc;
    Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> subOsc;
    ADSR<CONTROL_RATE, AUDIO_RATE> envelope;
    
    float frequency;
    float detuneAmount;  // Controls how much the second oscillator is detuned
    float subMixAmount;  // Controls the level of sub oscillator
    int detuneShift; 

    Oscil<SIN2048_NUM_CELLS, CONTROL_RATE> lfo;  // For vibrato
    float vibratoSpeed;
    float vibratoDepth;
    float vibratoMix; 
};

