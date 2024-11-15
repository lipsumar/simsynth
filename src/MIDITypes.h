#pragma once

#include <stdint.h>

// Just declare the type we need for note-to-frequency conversion
typedef float (*MIDIToFreqFn)(uint8_t);
