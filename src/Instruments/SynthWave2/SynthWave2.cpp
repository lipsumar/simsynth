#include "SynthWave2.h"

SynthwaveSynth::SynthwaveSynth(MIDIToFreqFn freqConverter) :
    Instrument(freqConverter),
    osc1(SAW2048_DATA),
    osc2(SAW2048_DATA),
    osc3(SAW2048_DATA),
    baseFreq(440.0f),
    detune(1.003f),
    filterCutoff(90),
    filterEnvAmount(0.6f),
    filterResonance(220)
{
    osc1.setFreq(0);
    osc2.setFreq(0);
    osc3.setFreq(0);
}

void SynthwaveSynth::setup() {
    // Setup amplitude envelope - moderate attack, long decay
    ampEnvelope.setADLevels(255, 180);
    ampEnvelope.setTimes(100, 300, 10000, 500);
    
    // Setup filter envelope - quick attack, long decay
    filterEnvelope.setADLevels(255, 0);
    filterEnvelope.setTimes(50, 800, 0, 800);
    
    // Initial filter setup
    lpf.setCutoffFreqAndResonance(filterCutoff, filterResonance);
}

void SynthwaveSynth::noteOn(uint8_t note, uint8_t velocity) {
    baseFreq = midiToFreq(note);
    
    // Set frequencies with detuning for rich sound
    osc1.setFreq((float)baseFreq);
    osc2.setFreq((float)(baseFreq * detune));      // Slightly sharp
    osc3.setFreq((float)(baseFreq * (1.0/detune))); // Slightly flat
    
    ampEnvelope.noteOn();
    filterEnvelope.noteOn();
}

void SynthwaveSynth::noteOff(uint8_t note, uint8_t velocity) {
    ampEnvelope.noteOff();
    filterEnvelope.noteOff();
}

void SynthwaveSynth::updateControl() {
    ampEnvelope.update();
    filterEnvelope.update();
    
    // Calculate filter cutoff with envelope modulation
    int envAmount = (int)(filterEnvelope.next() * filterEnvAmount);
    int currentCutoff = filterCutoff + envAmount;
    currentCutoff = constrain(currentCutoff, 0, 255);
    
    lpf.setCutoffFreqAndResonance(currentCutoff, filterResonance);
}

int16_t SynthwaveSynth::updateAudio() {
    // Mix all oscillators
    long signal = ((long)osc1.next() * 4 +  // Center oscillator slightly louder
              (long)osc2.next() * 3 + 
              (long)osc3.next() * 3) / 3;// Divide by 3 to prevent clipping
    
    // Apply filter
    int16_t filtered = lpf.next(signal);
    
    // Apply amplitude envelope
    filtered = (int16_t)((long)filtered * ampEnvelope.next() >> 5);
    
    return MonoOutput::from16Bit(filtered);
}

void SynthwaveSynth::setPot1Value(float value) {
    // Pot 1 controls detune amount (1.0 to 1.02)
    detune = 1.0f + (value * 0.04f); 
    // Update oscillator frequencies with new detune
    if (baseFreq > 0) {
        osc2.setFreq((float)(baseFreq * detune));
        osc3.setFreq((float)(baseFreq * (1.0/detune)));
    }
}

void SynthwaveSynth::setPot2Value(float value) {
    // Pot 2 controls filter cutoff (20 to 255)
    filterCutoff = 20 + (int)(value * 235);
}

void SynthwaveSynth::setPot3Value(float value) {
    // Pot 3 controls filter envelope amount
    filterEnvAmount = value;
}

void SynthwaveSynth::setEnvelope(uint8_t envNumber) {
    switch(envNumber) {
        case 0:  // Quick pluck
            ampEnvelope.setTimes(50, 200, 10000, 400);
            filterEnvelope.setTimes(30, 500, 0, 500);
            break;
        case 1:  // Classic synthwave pad
            ampEnvelope.setTimes(200, 800, 10000, 800);
            filterEnvelope.setTimes(150, 1200, 0, 1000);
            break;
        case 2:  // Long evolving
            ampEnvelope.setTimes(500, 2000, 10000, 1500);
            filterEnvelope.setTimes(400, 2000, 0, 1500);
            break;
    }
}