#include <SmingCore.h>
#include "GPIOStateManager.h"
#include "../Configuration/GPIOConfigProvider.h"
#include "../Services/Injector.h"


GPIOStateManager::GPIOStateManager(GPIOConfigProvider &cfgProvider) : cfgProvider(cfgProvider) {
	for(auto i=0; i<=PIN_MAX; i++) {
		timers[i] = nullptr;
	}
	config = cfgProvider.load();
}
GPIOStateManager::~GPIOStateManager() {
	for(auto i=0; i<=PIN_MAX; i++) {
		deleteTimer(i);
	}
	delete timers;
}

int GPIOStateManager::getPinByName(String name) {
	for(int i=0; i<=PIN_MAX; i++) {
		if(config.gpio[i].name == name) {
			return i;
		}
	}
	Serial.println("Invalid pin name " + name);
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
	int pin = getPinByName(pinName);
	if(pin<0) {
		return false;
	}
	Serial.println(pin);
	pinMode(pin, INPUT);
	Serial.println(isInputPin(pin) ? "Is input" : "is output");
	if(isInputPin(pin) || pin > PIN_MAX) {
		String msg = "Pin ";
		msg += pin;
		msg += " is not configured as output pin.";
		Serial.println(msg);
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
	config = cfgProvider.load();
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