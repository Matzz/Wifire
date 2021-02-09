#include <SmingCore.h>
#include "GPIOStateManager.h"


GPIOStateManager::GPIOStateManager(ConfigProvider<GPIOConfig>& cfgProvider) : cfgProvider(cfgProvider) {
	for(auto i=0; i<=GPIOConfig::max_pin; i++) {
		timers[i] = nullptr;
	}
}
GPIOStateManager::~GPIOStateManager() {
	for(auto i=0; i<=GPIOConfig::max_pin; i++) {
		deleteTimer(i);
	}
	delete timers;
}

int GPIOStateManager::getPinByName(GPIOConfig &config, String name) {
	for(int i=0; i<=GPIOConfig::max_pin; i++) {
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
bool GPIOStateManager::switchPin(String pinName, uint8_t state, uint32_t howLongMs) {
	state = state == HIGH ? HIGH : LOW;
	auto configOrError = cfgProvider.load();
	if(configOrError.is_left()) {
		debug_w("Configuration error: %s", configOrError.get_if_left()->c_str());
		return false;
	}
	auto config = *configOrError.get_if_right();

	int pin = getPinByName(config, pinName);
	if(pin<0) {
		return false;
	}
	if(isInputPin(pin)) {
		debug_w("Pin %s (%d) is not configured as output pin.", pinName, pin);
		return false;
	} else {
		digitalWrite(pin, state);
		debug_i("GPIO %s (%d) set to state %d. Requested time %d ms.", pinName, pin, state, howLongMs);
		if(howLongMs>0) {
			if(timers[pin] == nullptr) {
				timers[pin] = new Timer();
			}
			timers[pin]->initializeMs(howLongMs,  [pinName, pin, state]() {
				uint8_t oppositeState = state == HIGH ? LOW : HIGH;
				digitalWrite(pin, oppositeState);
				debug_i("GPIO %s (%d) reverted to state %d.", pinName, pin, oppositeState);
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
	auto configOrError = cfgProvider.load();
	if(configOrError.is_left()) {
		debug_w("Configuration error: %s", configOrError.get_if_left()->c_str());
		return;
	}
	auto config = *configOrError.get_if_right();
	
	for(int i=0; i<=GPIOConfig::max_pin; i++) {
		PinConfig pin = config.gpio[i];
		auto displayName = pin.name.c_str(); //  + String(" GPIO "); + String(i+'0')
		if(!GPIOConfig::isSafeToUse(i)) {
			debug_i("Not setting state for pin %s. Its unsafe to use.", displayName);
			continue;
		}
		debug_i("Setting pin %s to %s.", displayName, pin.isInput ? "input" : "output");
		if(pin.isInput) {
			pinMode(i, INPUT);
		} else {
			pinMode(i, OUTPUT);
		}
		debug_i("Setting pin %s pullup to %s.", displayName, pin.pull ? "pullup" : "no pullup");
		if(pin.pull) {
			pullup(i);
		} else {
			noPullup(i);
		}
	}
	debug_i("GPIOs state updated");
}