#include "Adafruit_NeoPixel.h"

#ifndef TAPE_H
#define TAPE_H

const int DEFAULT_BRIGHTNESS = 50;
const int DEFAULT_COLOR = 0xFFFFFF;
const int BLACK_COLOR = 0x000000;

class Tape {
  struct TapeSkipModel {
    int from;
    int to;
  };
  
  public:
    enum ShowingModeType { full, oneByOne, nightMode };
  
    Tape(int ledsCount, int pin, int brightness = DEFAULT_BRIGHTNESS, int color = DEFAULT_COLOR);
    void setupBrightness(int value);
    void setupColor(int value);
    void show();
    void showNightMode();
    void clearNightMode();
    void hide();
    void prepare();
    bool getIsShown();
    void update();

    void oneByOneModeEnable();
    void fullModeEnable();

   private:
    // for mode
    int _modeIndex = 0;
    bool _turningOn = true;
   
    void _show(bool isForce = false, ShowingModeType showingModeType = ShowingModeType::full);
    Adafruit_NeoPixel _strip;
    bool _isNightMode = false;
    int _ledsCount;
    int _pin;
    int _brightness;
    int _color;
    TapeSkipModel* _tapeSkipModels;
    int _tapeSkipModelsSize = 0;
    bool isShown = false;
    ShowingModeType _showingModelType = ShowingModeType::full;
};

#endif /* TAPE_H */
