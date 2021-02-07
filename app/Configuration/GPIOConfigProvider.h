#pragma once

#include <SmingCore.h>
#include "FileConfig.h"

struct PinConfig {
public:
	String name;
	bool isInput; // true = input, false = output
	bool pull;
};

class GPIOConfig {
public:
	static const unsigned int max_pin = 16;
	static const unsigned int safe_pins[];
	static bool isSafeToUse(unsigned int pin);
	PinConfig gpio[max_pin+1];
};

class GPIOConfigProvider: FileConfig {
public:
	GPIOConfigProvider(String fileName = "config/gpio.json");

	static GPIOConfig jsonToConfig(JsonObject& doc);
	static void configToJson(GPIOConfig& config, JsonObject& doc);

	GPIOConfig load();

	void save(GPIOConfig cfg);
};
