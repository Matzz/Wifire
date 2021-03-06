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

	void encode(JsonObject& json, GPIOConfig cfg) {
		JsonObject gpioObj = json.createNestedObject("gpio");
		for(int i=0; i<=GPIOConfig::max_pin; i++) {
			String key = String(i);
			JsonObject pinObj = gpioObj.createNestedObject(key);
			pinObj["name"] = cfg.gpio[i].name;
			bool isSafe = GPIOConfig::isSafeToUse(i);
			pinObj["isInput"] = isSafe ? cfg.gpio[i].isInput : false;
			pinObj["pull"] = isSafe ? cfg.gpio[i].pull : false;
		}
	}

	Either<String, GPIOConfig> decode(JsonObject& json) {
		GPIOConfig cfg;
		JsonObject gpioObj = json["gpio"].as<JsonObject>();
		for(unsigned int i=0; i<=GPIOConfig::max_pin; i++) {
			String key = String(i);
			if(gpioObj.containsKey(key)) {
				auto pinObj = gpioObj[key].as<JsonObject>();
				cfg.gpio[i] = {pinObj["name"].as<String>(), pinObj["isInput"].as<bool>(), pinObj["pull"].as<bool>()};
			} else {
				cfg.gpio[i] = {"GPIO_"+String(i), true, false};
			}
		}
		return {RightTagT(), cfg};
	}
};