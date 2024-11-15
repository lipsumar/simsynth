#include "ExponentialMapper.h"

ExponentialMapper::ExponentialMapper(float min, float max)
  : minValue(min),
    maxValue(max),
    logRatio(log(max / min)) {}

float ExponentialMapper::map(float normalized) {
  return minValue * exp(normalized * logRatio);
}
