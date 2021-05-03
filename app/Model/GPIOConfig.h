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
	String name = String::nullstr;
	bool isInput = true; // true = input, false = output
	bool pull = false;
};

class GPIOConfig {
public:
	static const unsigned int max_pin = 16;

	static constexpr unsigned int safe_pins[] = SAFE_PINS;
	static bool isSafeToUse(unsigned int pin);
	PinConfig gpio[max_pin+1];
};

template<>
void Codec<GPIOConfig>::encode(JsonObject& json, const GPIOConfig &cfg);

template<>
Either<String, GPIOConfig> Codec<GPIOConfig>::decode(JsonObject& json);