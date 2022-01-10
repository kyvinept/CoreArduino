#include "WifiServer.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

class TimeManager {
  public: 
    struct TimeModel {
      int hours;
      int minutes;
      int seconds;
    };

    struct TimeWorkModel {
      TimeModel startTime;
      TimeModel endTime;
    };
  
    void (*onStartTime)(void);
    void (*onEndTime)(void);

    TimeManager();
    void subscribeToTime(TimeWorkModel timeWorkModel);
    void update();
    void clear(int startHours, int startMinutes, int endHours, int endMinutes);
    
  private:
    WiFiUDP _ntpUDP;
    NTPClient* _timeClient;
    bool _triggeredStart = false;
    bool _isSettedTime = false;

    TimeWorkModel* _timeWorkModels = new TimeWorkModel[2];
    int _timeWorkModelIndex = 0;

    void _checkTimeUpdate(TimeModel startTime, TimeModel endTime, int nowHours, int nowMinutes, int nowSeconds);
};

#endif /* TIMEMANAGER_H */
