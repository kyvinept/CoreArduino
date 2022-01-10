#include "Magnet.h"

Magnet::Magnet(int pin) {
  _pin = pin;
  
  pinMode(_pin, INPUT_PULLUP);
}

void Magnet::update() {
  int magnetSignal = digitalRead(_pin);
  Serial.println(magnetSignal);

  if (!_setuped) {
    _prevSignal = magnetSignal;
    _setuped = true;
    return;
  }

  if (_prevSignal == magnetSignal) {
    return;
  }

  if (magnetSignal == LOW) {
    onMagnetClosed();
  } else {
    onMagnetOpened();
  }

  _prevSignal = magnetSignal;
}
