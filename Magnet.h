#include "Adafruit_NeoPixel.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet {
  private:
    int _pin;
    int _prevSignal = -1;
    boolean _setuped = false;

  public:
    void (*onMagnetOpened)(void);
    void (*onMagnetClosed)(void);
  
    Magnet(int pin);
    void update();
};

#endif /* MAGNET_H */
