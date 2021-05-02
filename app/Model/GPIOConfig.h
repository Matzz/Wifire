#pragma once

#include <SmingCore.h>
#include "Codec.h"


#if defined(ARCH_ESP8266) | defined(ARCH_HOST) 
#define SAFE_PINS {2,4,5,12,13,14,15,16}
#define MAX_PIN 16
#elif defined(ARCH_ESP32)
// TODO set correct pins for ESP32
#define SAFE_PINS {2,4,5,12,13,14,15,16,,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33}
#define MAX_PIN 39
#endif

struct PinConfig {
public:
	String name;
	bool isInput; // true = input, false = output
	bool pull;
};

class GPIOConfig {
public:
	static const unsigned int max_pin = 16;

	static constexpr unsigned int safe_pins[] = SAFE_PINS;
	static bool isSafeToUse(unsigned int pin);
	PinConfig gpio[max_pin+1];
};

template<> class Codec<GPIOConfig> {
	public:
        static Codec<GPIOConfig>& getInstance() {
            static Codec<GPIOConfig> instance;
            return instance;
        }

	void encode(JsonObject& json, const GPIOConfig &cfg) {
		JsonObject gpioObj = json.createNestedObject("gpio");
		for(int i=0; i<=GPIOConfig::max_pin; i++) {
			String key = String(i);
			JsonObject pinObj = gpioObj.createNestedObject(key);
			pinObj["name"] = cfg.gpio[i].name;
			pinObj["isInput"] = cfg.gpio[i].isInput;
			pinObj["pull"] = cfg.gpio[i].pull;
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