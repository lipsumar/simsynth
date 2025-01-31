#pragma once


#include <stdint.h>
#include "MIDITypes.h"

class Instrument {
public:
  Instrument(MIDIToFreqFn freqConverter)
    : midiToFreq(freqConverter) {}
  virtual ~Instrument() = default;

  virtual void setup() = 0;
  virtual void noteOn(uint8_t note, uint8_t velocity) = 0;
  virtual void noteOff(uint8_t note, uint8_t velocity) = 0;
  virtual void updateControl() = 0;
  virtual int16_t updateAudio() = 0;

  // Generic parameter setters - instruments will implement their own logic
  virtual void setPot1Value(float value) = 0;
  virtual void setPot2Value(float value) = 0;
  virtual void setPot3Value(float value) = 0;
  virtual void setEnvelope(uint8_t envNumber) = 0;

protected:
  MIDIToFreqFn midiToFreq;
};