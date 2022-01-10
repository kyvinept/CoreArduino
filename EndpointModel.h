#import <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <map>

#ifndef ENDPOINTMODEL_H
#define ENDPOINTMODEL_H

struct EndpointModel {
  char* requestName;
  HTTPMethod requestType;
  std::map<char*, char*> (*callback)(StaticJsonDocument<200> json);
};

#endif /* ENDPOINTMODEL_H */
