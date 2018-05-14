#ifndef APP_ACTIONS_CONFIGACTIONS_H_
#define APP_ACTIONS_CONFIGACTIONS_H_
#include <SmingCore/SmingCore.h>

void apGetConfigAction(HttpRequest &request, HttpResponse &response);
void apSetConfigAction(HttpRequest &request, HttpResponse &response);
void stationGetConfigAction(HttpRequest &request, HttpResponse &response);
void stationSetConfigAction(HttpRequest &request, HttpResponse &response);
void otaGetConfigAction(HttpRequest &request, HttpResponse &response);
void otaSetConfigAction(HttpRequest &request, HttpResponse &response);

#endif
