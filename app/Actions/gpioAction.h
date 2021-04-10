#pragma once

#include <SmingCore.h>
#include <JsonObjectStream.h>
#include "actionsHelpers.h"
#include "../Model/GPIOConfig.h"

void currentState(ConfigProvider<GPIOConfig> &cfgProvider, JsonDocument& doc);
void addSafePinsToJson(JsonDocument& doc);

void gpioGetConfigAction(HttpRequest &request, HttpResponse &response);
void gpioSetConfigAction(HttpRequest &request, HttpResponse &response);

void gpioSwitchAction(HttpRequest &request, HttpResponse &response);
void gpioSwitchGetAvailableAction(HttpRequest &request, HttpResponse &response);
