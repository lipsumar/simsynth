#pragma once
#include <stdint.h>
#include "../MovingAverage.h"
#include "../ExponentialMapper/ExponentialMapper.h"

// Forward declare mozziAnalogRead without including MozziGuts.h
template<uint8_t RES> uint16_t mozziAnalogRead(uint8_t pin);

class PotController {
private:
  const int pin;
  MovingAverage<8> average;
  ExponentialMapper mapper;

public:
  PotController(int potPin, float minVal, float maxVal);
  float read() {
    int raw = mozziAnalogRead<12>(pin);
    int averaged = average.update(raw);
    float normalized = (float)averaged / 1023.0f;
    return mapper.map(normalized);
  }
};