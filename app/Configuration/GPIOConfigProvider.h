#pragma once

#include <SmingCore.h>
#include "FileConfig.h"

#define PIN_MAX 16

struct PinConfig {
public:
	String name;
	bool isInput; // true = input, false = output
	bool pull;
};

class GPIOConfig {
public:
	PinConfig gpio[PIN_MAX+1];
};

class GPIOConfigProvider: FileConfig {
public:
	GPIOConfigProvider(String fileName = "config/gpio.json");

	static GPIOConfig jsonToConfig(JsonObject& doc);
	static void configToJson(GPIOConfig& config, JsonObject& doc);

	GPIOConfig load();

	void save(GPIOConfig cfg);
};
