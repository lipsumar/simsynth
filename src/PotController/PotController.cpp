#include "PotController.h"

PotController::PotController(int potPin, float minVal, float maxVal)
  : pin(potPin),
    mapper(minVal, maxVal) {}


