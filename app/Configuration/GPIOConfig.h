#pragma once

#include <SmingCore.h>
#include "Codec.h"

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

template<> class Codec<GPIOConfig> {
	public:
        static Codec<GPIOConfig>& getInstance() {
            static Codec<GPIOConfig> instance;
            return instance;
        }

	void encode(JsonObject& json, GPIOConfig obj) {
		JsonArray gpioArr = json.createNestedArray("gpio");
		for(int i=0; i<=GPIOConfig::max_pin; i++) {
			JsonObject pinObj = gpioArr.createNestedObject();
			pinObj["name"] = obj.gpio[i].name;
			bool isSafe = GPIOConfig::isSafeToUse(i);
			pinObj["isInput"] = isSafe ? obj.gpio[i].isInput : false;
			pinObj["pull"] = isSafe ? obj.gpio[i].pull : false;
		}
	}

	GPIOConfig decode(JsonObject& json) {
		GPIOConfig cfg;
		JsonArray gpioArr = json["gpio"].as<JsonArray>();
		int gpioArrSize = gpioArr.size();
		for(int i=0; i<=GPIOConfig::max_pin; i++) {
			if(i<gpioArrSize) {
				auto gpioObj = gpioArr[i].as<JsonObject>();
				cfg.gpio[i] = {gpioObj["name"].as<String>(), gpioObj["isInput"].as<bool>(), gpioObj["pull"].as<bool>()};
			} else {
				cfg.gpio[i] = {"GPIO_"+String(i), true, false};
			}
		}
		return cfg;
	}
};