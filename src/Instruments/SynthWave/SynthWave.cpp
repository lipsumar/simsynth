#include "SynthWave.h"

SynthWave::SynthWave(MIDIToFreqFn freqConverter) :
    Instrument(freqConverter),
    mainOsc(SAW2048_DATA),
    mainOsc2(SAW2048_DATA),
    subOsc(SQUARE_NO_ALIAS_2048_DATA),
    frequency(440.0),
    scaledCutoff(180),
    resonanceAmount(100),
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
    mainOsc2.setFreq(frequency * 1.0005f );
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
    long main = ((long)mainOsc.next() + mainOsc2.next()) >> 1;
    long sub = (long)(subOsc.next() * subMixAmount) >> 1; 

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
   //subMixAmount = value;
}

void SynthWave::setPot3Value(float value) {
    
}

void SynthWave::setEnvelope(byte envNumber) {
  if(envNumber==0){
    envelope.setTimes(30, 30, 10000, 30);
  } else if (envNumber==1){
    envelope.setTimes(50, 200, 10000, 400);
  }else{
    envelope.setTimes(800, 800, 10000, 5000);
  }
}