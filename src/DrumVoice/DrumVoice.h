#pragma once

#include "../../SIMSynthConfig.h"
#include <Sample.h>
#include "../samples/kick.h"

template<uint16_t NUM_CELLS>
class DrumVoice {
private:
  Sample<NUM_CELLS, AUDIO_RATE>* sample;
  const uint8_t* currentPattern;
  uint8_t patternLength;
  bool isPlaying;
  bool isEngaged;

  // Array of patterns and its size
  const uint8_t** patterns;
  uint8_t numPatterns;
  uint8_t currentPatternIndex;

public:
  DrumVoice(Sample<NUM_CELLS, MOZZI_AUDIO_RATE>* _sample, const uint8_t** _patterns, uint8_t _numPatterns)
    : sample(_sample), patterns(_patterns), numPatterns(_numPatterns),
      isPlaying(false), isEngaged(false), currentPatternIndex(0) {
    currentPattern = patterns[0];
    patternLength = 16;  // Default pattern length

    // float freq = 1.11;//((float)AUDIO_RATE / (float)BD_SAMPLERATE) * ((float)BD_SAMPLERATE / (float)BD_NUM_CELLS);
    sample->setFreq(((float)AUDIO_RATE / (float)BD_SAMPLERATE) * ((float)BD_SAMPLERATE / (float)BD_NUM_CELLS));
  }

  void changePattern() {
    currentPatternIndex = (currentPatternIndex + 1) % numPatterns;
    currentPattern = patterns[currentPatternIndex];
  }

  // Called when the metronome ticks
  void tick(uint8_t currentStep, bool isEnabled) {

    if (isEnabled && !isEngaged && !isPlaying) {
      isEngaged = true;
    }
    if (!isEnabled && isPlaying) {
      isPlaying = false;
    }

    if (isEngaged && currentStep == 0) {
      changePattern();
      isPlaying = true;
      isEngaged = false;
    }

    if (isPlaying && currentPattern[currentStep] == 1) {
      sample->start();
    }
  }

  int16_t updateAudio() {
    if (!isPlaying) return 0;
    //return (sample->next() * 255);
    int asig = (int)((long)sample->next() * 255) >> 3;
    return asig;
  }
};