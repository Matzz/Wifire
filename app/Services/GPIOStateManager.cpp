#include <SmingCore.h>
#include "GPIOStateManager.h"
#include "../Configuration/GPIOConfigProvider.h"
#include "../Services/Injector.h"


GPIOStateManager::GPIOStateManager(GPIOConfigProvider& cfgProvider) : cfgProvider(cfgProvider) {
	for(auto i=0; i<=PIN_MAX; i++) {
		timers[i] = nullptr;
	}
}
GPIOStateManager::~GPIOStateManager() {
	for(auto i=0; i<=PIN_MAX; i++) {
		deleteTimer(i);
	}
	delete timers;
}

int GPIOStateManager::getPinByName(GPIOConfig &config, String name) {
	for(int i=0; i<=PIN_MAX; i++) {
		if(config.gpio[i].name == name) {
			return i;
		}
	}
	debug_w("Invalid pin name %s", name.c_str());
	return -1;
}

void GPIOStateManager::deleteTimer(int pin) {
	if(timers[pin] != nullptr) {
		timers[pin]->stop();
		delete timers[pin];
		timers[pin] = nullptr;
	}
}

bool GPIOStateManager::turnOn(String pinName, unsigned int howLong) {
	auto config = cfgProvider.load();
	int pin = getPinByName(config, pinName);
	if(pin<0) {
		return false;
	}
	if(isInputPin(pin) || pin > PIN_MAX) {
		debug_w("Pin %s (%d) is not configured as output pin.", pinName.c_str(), pin);
		return false;
	} else {
		digitalWrite(pin, 1);
		if(howLong>0) {
			if(timers[pin] == nullptr) {
				timers[pin] = new Timer();
			}
			timers[pin]->initializeMs(howLong,  [pin]() {
				digitalWrite(pin, 0);
				//This doesn't work in lambda, seg fault emited. TODO - fix it
				//deleteTimer(pin);
			});
			timers[pin]->startOnce();
		} else if(timers[pin] != nullptr) {
			deleteTimer(pin);
		}
		return true;
	}

}

void GPIOStateManager::update() {
	auto config = cfgProvider.load();
	for(int i=0; i<=PIN_MAX; i++) {
		auto pin = config.gpio[i];
		if(pin.isInput) {
			pinMode(i, INPUT);
		} else {
			pinMode(i, OUTPUT);
		}
		if(pin.pull) {
			pullup(i);
		} else {
			noPullup(i);
		}
	}
}