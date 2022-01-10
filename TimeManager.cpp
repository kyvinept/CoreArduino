#include "TimeManager.h"

TimeManager::TimeManager() {
  _timeClient = new NTPClient(_ntpUDP, "pool.ntp.org");
  _timeClient->begin();
}

void TimeManager::update() {
  if (_isSettedTime) {
    _timeClient->update();

    int nowHours = _timeClient->getHours();
    int nowMinutes = _timeClient->getMinutes();
    int nowSeconds = _timeClient->getSeconds();

    for(int index = 0; index < _timeWorkModelIndex; index++) {
      _checkTimeUpdate(_timeWorkModels[index].startTime, _timeWorkModels[index].endTime, nowHours, nowMinutes, nowSeconds);
    }
  }
}

void TimeManager::_checkTimeUpdate(TimeModel startTime, TimeModel endTime, int nowHours, int nowMinutes, int nowSeconds) {
  bool startTimeCondition = nowHours > startTime.hours || (nowHours == startTime.hours && nowMinutes >= startTime.minutes);
  bool endTimeCondition = nowHours > endTime.hours || (nowHours == endTime.hours && nowMinutes >= endTime.minutes);
  bool isStartTimeBiggerThanEndTime = startTime.hours > endTime.hours || (startTime.hours == endTime.hours && startTime.minutes > endTime.minutes) || (startTime.hours == endTime.hours && startTime.minutes == endTime.minutes && startTime.seconds >= endTime.seconds);

  if (!_triggeredStart) {
    if (startTimeCondition && !endTimeCondition) {
      if (onStartTime) {
        onStartTime();
        _triggeredStart = true;
      }
    } else if (isStartTimeBiggerThanEndTime && !endTimeCondition) {
      if (onStartTime) {
        onStartTime();
        _triggeredStart = true;
      }
    }
  } else {
    if (endTimeCondition) {
      if (onEndTime) {
        onEndTime();
        _triggeredStart = false;
      }
    }
  }
}

void TimeManager::subscribeToTime(TimeWorkModel timeWorkModel) {
  _timeWorkModels[_timeWorkModelIndex] = timeWorkModel;
  _isSettedTime = true;
  _timeWorkModelIndex++;
}

void TimeManager::clear(int startHours, int startMinutes, int endHours, int endMinutes) {
  TimeWorkModel* newTimeWorkModels = new TimeWorkModel[2];
  int currentIndex = 0;
  for(int index = 0; index < _timeWorkModelIndex; index++) {
    TimeWorkModel timeWorkModel = _timeWorkModels[index];
    if (!(timeWorkModel.startTime.hours == startHours && timeWorkModel.startTime.minutes == startMinutes && timeWorkModel.endTime.hours == endHours && timeWorkModel.endTime.minutes == endMinutes)) {
      newTimeWorkModels[currentIndex] = timeWorkModel;
      currentIndex++;
    }
  }
  
  if (currentIndex == 0) {
    _isSettedTime = false;
  }

  _timeWorkModelIndex = currentIndex;
  _timeWorkModels = newTimeWorkModels;

  int nowHours = _timeClient->getHours();
  int nowMinutes = _timeClient->getMinutes();
  bool startTimeCondition = nowHours > startHours || (nowHours == startHours && nowMinutes >= startMinutes);
  bool endTimeCondition = nowHours > endHours || (nowHours == endHours && nowMinutes >= endMinutes);
  if (startTimeCondition && !endTimeCondition && onEndTime) {
     onEndTime();
     _triggeredStart = false;
  }
}
