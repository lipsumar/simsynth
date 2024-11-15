#pragma once

template<int SIZE>
class MovingAverage {
private:
  int readings[SIZE] = { 0 };
  int readIndex = 0;
  long total = 0;

public:
  int update(int newValue) {
    total = total - readings[readIndex];
    readings[readIndex] = newValue;
    total = total + newValue;
    readIndex = (readIndex + 1) % SIZE;
    return total / SIZE;
  }
};
