#include "GentleSynth.h"

// In constructor, initialize:
GentleSine::GentleSine(MIDIToFreqFn freqConverter) :
    Instrument(freqConverter),
    mainOsc(SIN2048_DATA),
    detuneOsc(SIN2048_DATA),
    subOsc(SIN2048_DATA),
    lfo(SIN2048_DATA),
    frequency(440.0f),
    vibratoSpeed(5.0f),    // 5 Hz default
    vibratoDepth(0.0f),     
    vibratoMix(0.1f) // Start with no vibrato
{
    // Lock in the sweet spot mix values
    detuneShift = 1;       // Or whatever value gave the nice sound
    subMixAmount = 0.5f;   // The sweet spot we found
    lfo.setFreq(vibratoSpeed);
}

void GentleSine::setup() {
    // Start with gentle envelope settings
    envelope.setADLevels(255, 200);
    envelope.setTimes(100, 300, 10000, 500);
}

void GentleSine::noteOn(byte note, byte velocity) {
    frequency = midiToFreq(note);
    mainOsc.setFreq(frequency);
    detuneOsc.setFreq(frequency * 2.0f);  // Octave up instead of slight detune
    subOsc.setFreq(frequency * 0.5f);     // Still one octave down
    envelope.noteOn();
}

void GentleSine::noteOff(byte note, byte velocity) {
    envelope.noteOff();
}


void GentleSine::updateControl() {
    envelope.update();
    
    // Apply vibrato, now scaled by vibratoMix
    float vibrato = 1.0f + (lfo.next() * vibratoDepth * 0.01f * (vibratoMix-0.1));  
    mainOsc.setFreq(frequency * vibrato);
    detuneOsc.setFreq(frequency * 2.0f * vibrato);  
    subOsc.setFreq(frequency * 0.5f * vibrato);     
}

int16_t GentleSine::updateAudio() {
    // Get base signals
    int main = mainOsc.next();
    int detune = detuneOsc.next();
    int sub = subOsc.next();
    
    long mixed = ((long)main << 2)                    // Main osc base volume
                + ((long)detune >> detuneShift)       // Variable octave-up level
                + (long)(sub * subMixAmount);         // Sub osc with its mix amount

    mixed = mixed >> 1;                              // Final scaling before envelope
    mixed = (mixed * envelope.next()) >> 8;
    
    return mixed;
}


void GentleSine::setPot1Value(float value) {
    // Pot 1 controls vibrato speed from 0.5 to 8 Hz
    vibratoSpeed = 0.5f + (value * 7.5f);
    lfo.setFreq(vibratoSpeed);
}

void GentleSine::setPot2Value(float value) {
    // Pot 2 controls vibrato depth
    vibratoDepth = value;
}

void GentleSine::setPot3Value(float value) {

    // Master vibrato control - from none to full effect
    vibratoMix = value;
}

void GentleSine::setEnvelope(byte envNumber) {
    switch(envNumber) {
        case 0:  // Quick, pad-like
            envelope.setTimes(100, 300, 10000, 500);
            break;
        case 1:  // Slower, more atmospheric
            envelope.setTimes(500, 1000, 10000, 2000);
            break;
        case 2:  // Very slow, ambient
            envelope.setTimes(2000, 3000, 10000, 4000);
            break;
    }
}