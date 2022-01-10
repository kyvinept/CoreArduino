#include "Tape.h"

Tape::Tape(int ledsCount, int pin, int brightness, int color) {
  _brightness = brightness;
  _color = color;
  _ledsCount = ledsCount;
  _pin = pin;
  _strip = Adafruit_NeoPixel(ledsCount, pin, NEO_GRB + NEO_KHZ800);
}

bool Tape::getIsShown() {
  return isShown;
}

void Tape::prepare() {
  _strip.begin();
  _strip.setBrightness(_brightness);
  _strip.clear();
  _strip.show();
}

void Tape::setupBrightness(int value) {
  _brightness = value;
  _strip.setBrightness(value);
  
  if (isShown) {
    _show();
  }
}

void Tape::setupColor(int value) {
  _color = value;

  if (isShown) {
    _show();
  }
}

void Tape::show() {
  isShown = true;
  _show();
}

void Tape::update() {
  if (!isShown) {
    return;
  }
  
  switch (_showingModelType) {
    case Tape::ShowingModeType::oneByOne:
      if (_modeIndex >= _ledsCount) {
        _modeIndex = 0;
        _turningOn = !_turningOn;
      }
    
      _strip.setPixelColor(_modeIndex, _turningOn ? _color : BLACK_COLOR);
      _strip.show();
      _modeIndex++;
      break;
  }
}

void Tape::_show(bool isForce, ShowingModeType showingModeType) {
  switch (isForce ? showingModeType : _showingModelType) {
    case Tape::ShowingModeType::nightMode:
      for (int j = 0; j < _ledsCount; j++) {
        bool isCovered = false;
        for (int i = 0; i < _tapeSkipModelsSize; i++) {
          if (j >= _tapeSkipModels[i].from && j <= _tapeSkipModels[i].to) {
            isCovered = true;
          }
        }
  
        if (isCovered) {
          _strip.setPixelColor(j, _color);
        } else {
          _strip.setPixelColor(j, BLACK_COLOR);
        }
      }

      _strip.show();
      break;

    case Tape::ShowingModeType::full:
      for (int i = 0; i < _ledsCount; i++) {
        _strip.setPixelColor(i, _color);
      }
      _strip.show();
      break;
  }
}

void Tape::showNightMode() {
  _isNightMode = true;
  setupBrightness(5);
  _tapeSkipModelsSize = _ledsCount / 3;
  TapeSkipModel* tapeSkipModels = new TapeSkipModel[_tapeSkipModelsSize];
  for (int i = 0, value = 0; i < _tapeSkipModelsSize && value < _ledsCount; i++, value+=3) {
    tapeSkipModels[i].from = value;
    tapeSkipModels[i].to = value;
  }
  tapeSkipModels[5].to = 10;
  _tapeSkipModels = tapeSkipModels;

  _show(true, ShowingModeType::nightMode);
}

void Tape::clearNightMode() {
  _isNightMode = false;
  hide();
  setupBrightness(_brightness);
}

void Tape::oneByOneModeEnable() {
  _showingModelType = ShowingModeType::oneByOne;
}

void Tape::fullModeEnable() {
  _showingModelType = ShowingModeType::full;
}

void Tape::hide() {
  isShown = false;

  // for mode
  _turningOn = true;

  if (_isNightMode) {
    setupBrightness(_brightness);
    showNightMode();
  } else {
    _strip.clear();
    _strip.show();
  }
}
