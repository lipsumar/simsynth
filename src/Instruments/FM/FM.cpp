#include "../../../SIMSynthConfig.h"
#include "FM.h"


FMInstrument::FMInstrument(MIDIToFreqFn freqConverter)
  : Instrument(freqConverter),
    oscCarrier(SIN2048_DATA),
    oscModulator1(SIN2048_DATA),
    oscModulator2(SAW2048_DATA),
    mod_ratio2(4.97f),  // Fixed non-integer ratio for second modulator
    currentNote(0),
    isNoteOn(false) {
}

void FMInstrument::setup() {
  envelope.setADLevels(255, 127);
  envelope.setTimes(20, 200, 10000, 800);

  // Initialize parameters
  mod_intensity = 1000;  // Default modulation intensity
  mod_ratio1 = 2.0f;     // Default ratio for first modulator
}

void FMInstrument::noteOn(uint8_t note, uint8_t velocity) {
  currentNote = note;
  float frequency = midiToFreq(note);

  oscCarrier.setFreq(frequency);
  oscModulator1.setFreq(frequency * mod_ratio1);
  oscModulator2.setFreq(frequency * mod_ratio2);

  envelope.noteOn();
  isNoteOn = true;
}

void FMInstrument::noteOff(uint8_t note, uint8_t velocity) {
  envelope.noteOff();
  isNoteOn = false;
}

void FMInstrument::updateControl() {
  envelope.update();
}

int16_t FMInstrument::updateAudio() {
  envelope.update();
  // Combine both modulators
  long modulation1 = mod_intensity * oscModulator1.next();
  long modulation2 = (mod_intensity >> 1) * oscModulator2.next();  // Slightly less intense

  // Apply combined modulation
  int8_t carrier = oscCarrier.phMod(modulation1 + modulation2);

  return MonoOutput::from16Bit(carrier * envelope.next());
}

void FMInstrument::setPot1Value(float value) {
  // Map value (0.0-1.0) to modulation intensity range
  mod_intensity = static_cast<long>(value * 2000.0f);  // Range: 0-2000
}

void FMInstrument::setPot2Value(float value) {
  // Map value (0.0-1.0) to ratio range
  mod_ratio1 = 0.5f + (value * 7.33f);  // Range: 0.5-7.83

  // Update frequency if a note is currently playing
  //if (isNoteOn) {
  float frequency = midiToFreq(currentNote);
  oscModulator1.setFreq(frequency * mod_ratio1);
  //}
}