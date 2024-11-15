#include "SynthWave.h"

SynthWave::SynthWave(MIDIToFreqFn freqConverter) :
    Instrument(freqConverter),
    mainOsc(SAW2048_DATA),
    mainOsc2(SAW2048_DATA),
    subOsc(SQUARE_NO_ALIAS_2048_DATA),
    frequency(440.0),
    scaledCutoff(180),
    resonanceAmount(180),
    subMixAmount(0.7)
{
}


void SynthWave::setup() {
    envelope.setADLevels(255, 180);
    envelope.setTimes(50, 200, 10000, 200);
    lpf.setCutoffFreqAndResonance(scaledCutoff, resonanceAmount);
}

void SynthWave::noteOn(byte note, byte velocity) {
    frequency = midiToFreq(note);
    mainOsc.setFreq(frequency);
    mainOsc2.setFreq(frequency * 1.001f);
    subOsc.setFreq(frequency * 0.5f);
    envelope.noteOn();
}

void SynthWave::noteOff(byte note, byte velocity) {
    envelope.noteOff();
}

void SynthWave::updateControl() {
    // Only update envelope - pot reading happens in main sketch
}

int16_t SynthWave::updateAudio() {
    envelope.update();

    // Mix oscillators
    long main = (long)mainOsc.next() + mainOsc2.next();
    long sub = ((long)subOsc.next() * subMixAmount);

    // Combine signals
    int signal = (main + sub) >> 3;

    // Apply filter and envelope
    signal = lpf.next(signal);
    signal = (int)((long)signal * envelope.next());

    return MonoOutput::from16Bit(signal);
}

void SynthWave::setPot1Value(float value) {
    // Map incoming 0-1 value to filter range
    scaledCutoff = (int)(value * 245) + 10; // Ensures range of 10-255
    lpf.setCutoffFreqAndResonance(scaledCutoff, resonanceAmount);
}

void SynthWave::setPot2Value(float value) {
    subMixAmount = value;
}