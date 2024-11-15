#pragma once
#include <math.h>  // for exp() and log()

class ExponentialMapper {
private:
  float minValue;
  float maxValue;
  float logRatio;

public:
  ExponentialMapper(float min, float max);
  float map(float normalized);
};