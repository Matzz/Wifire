#pragma once

#include <SmingCore.h>
#include "actionsHelpers.h"

void apGetConfigAction(HttpRequest &request, HttpResponse &response);
void apSetConfigAction(HttpRequest &request, HttpResponse &response);

void stationGetConfigAction(HttpRequest &request, HttpResponse &response);
void stationSetConfigAction(HttpRequest &request, HttpResponse &response);

void stationRefreshNetworks(HttpRequest &request, HttpResponse &response);
void stationGetNetworks(HttpRequest &request, HttpResponse &response);

void otaGetConfigAction(HttpRequest &request, HttpResponse &response);
void otaSetConfigAction(HttpRequest &request, HttpResponse &response);

void gpioGetConfigAction(HttpRequest &request, HttpResponse &response);
void gpioSetConfigAction(HttpRequest &request, HttpResponse &response);
