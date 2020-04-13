#pragma once

#include <SmingCore.h>
#include "FileConfig.h"

#define PIN_MAX 16

class GPIOConfig {
public:
	uint8 modes[PIN_MAX];
	bool pullUps[PIN_MAX];
	String names[PIN_MAX];
};

class GPIOConfigProvider: FileConfig {
public:
	GPIOConfigProvider(String fileName = "config/gpio.json");

	GPIOConfig load();

	void save(GPIOConfig cfg);
};
