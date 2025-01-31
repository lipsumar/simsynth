#include "ChaosSynth.h"

ChaosSynth::ChaosSynth(MIDIToFreqFn freqConverter) :
    Instrument(freqConverter),
    mainOsc(SAW2048_DATA),
    ringMod(SQUARE_NO_ALIAS_2048_DATA),
    lfoFilter(TRIANGLE2048_DATA),
    baseFreq(440.0f),
    filterCutoff(128),
    ringModAmount(0.0f),
    filterLfoAmount(0.0f),
    currentLfoValue(0.0f)
{
  // Explicitly initialize oscillators to a safe frequency
    mainOsc.setFreq(0);
    ringMod.setFreq(0);
    lfoFilter.setFreq(0); // 2 Hz LFO for filter
}

void ChaosSynth::setup() {
    envelope.setADLevels(255, 180);
    envelope.setTimes(50, 200, 10000, 200);
    lpf.setCutoffFreqAndResonance(filterCutoff, 200); // High resonance for squelchy sounds
}

void ChaosSynth::noteOn(uint8_t note, uint8_t velocity) {
    baseFreq = midiToFreq(note);
    mainOsc.setFreq(baseFreq);
    ringMod.setFreq(baseFreq * 2.0f); // Ring mod at 2x frequency
    envelope.noteOn();
}

void ChaosSynth::noteOff(uint8_t note, uint8_t velocity) {
    envelope.noteOff();
}

void ChaosSynth::updateControl() {
    envelope.update();
    // Update LFO value for filter modulation
    currentLfoValue = (float)lfoFilter.next() / 128.0f;
    
    // Calculate filter cutoff with LFO modulation
    int modulatedCutoff = filterCutoff + (int)(currentLfoValue * filterLfoAmount * 128);
    modulatedCutoff = constrain(modulatedCutoff, 0, 255);
    lpf.setCutoffFreqAndResonance(modulatedCutoff, 200);
}

int16_t ChaosSynth::updateAudio() {
    
    
    // Get main oscillator signal
    long mainSignal = (long)mainOsc.next() * 8;
    
    // Get secondary osc signal and apply XOR modulation
    // This creates harsh digital distortion that changes timbre dramatically
    if (ringModAmount > 0) {  // we'll keep the variable name for simplicity
        long modSignal = (long)ringMod.next() * 4;
        // Bitwise operations create harsh digital effects
        long crushedSignal = (mainSignal & modSignal) + (mainSignal ^ modSignal);
        // Mix between clean and crushed
        mainSignal = (crushedSignal * (long)ringModAmount + mainSignal * (255 - (long)ringModAmount)) >> 8;
    }
    
    // Apply filter
    int16_t filtered = lpf.next(mainSignal);
    
    // Apply envelope
    filtered = (int16_t)((long)filtered * envelope.next() >> 6);
    
    return MonoOutput::from16Bit(filtered);
}

void ChaosSynth::setPot1Value(float value) {
    // Pot 1 controls ring modulation amount (0 to full)
    ringModAmount = value * 255;
}

void ChaosSynth::setPot2Value(float value) {
    // Pot 2 controls filter cutoff base value
    filterCutoff = (int)(value * 255);
}

void ChaosSynth::setPot3Value(float value) {
    // Pot 3 controls filter LFO amount
    filterLfoAmount = value;
    // Also adjust LFO speed based on value
    lfoFilter.setFreq(1.0f + (value * 10.0f)); // LFO speed 1-11 Hz
}

void ChaosSynth::setEnvelope(uint8_t envNumber) {
    switch(envNumber) {
        case 0:  // Percussive
            envelope.setTimes(30, 100, 10000, 200);
            break;
        case 1:  // Medium decay
            envelope.setTimes(50, 400, 10000, 400);
            break;
        case 2:  // Long, sweeping
            envelope.setTimes(100, 1000, 10000, 800);
            break;
    }
}