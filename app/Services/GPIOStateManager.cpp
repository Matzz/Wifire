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


/**
https://tttapa.github.io/ESP8266/Chap04%20-%20Microcontroller.html
Label	GPIO	Input	Output	Notes
A0	ADC0	Analog Input	X	
D3	GPIO0	pulled up	OK	connected to FLASH button, boot fails if pulled LOW
TX	GPIO1	TX pin	OK	HIGH at boot debug output at boot, boot fails if pulled LOW
D4	GPIO2	pulled up	OK	HIGH at boot connected to on-board LED, boot fails if pulled LOW
RX	GPIO3	OK	RX pin	HIGH at boot
D2	GPIO4	OK	OK	often used as SDA (I2C)
D1	GPIO5	OK	OK	often used as SCL (I2C)
D6	GPIO12	OK	OK	SPI (MISO)
D7	GPIO13	OK	OK	SPI (MOSI)
D5	GPIO14	OK	OK	SPI (SCLK)
D8	GPIO15	pulled to GND	OK	SPI (CS) Boot fails if pulled HIGH
D0	GPIO16	no interrupt	no PWM or I2C support	HIGH at boot used to wake up from deep sleep
*/
bool GPIOStateManager::isSafeToUse(unsigned int pin) {
	switch(pin) {
		case 0:
		case 1: return false;
		case 2:
		case 3:
		case 4:
		case 5: return true;
		case 6: 
		case 7: 
		case 8: 
		case 9: 
		case 10: 
		case 11: 
		case 12: return false;
		case 13:
		case 14: 
		case 15:
		case 16: return true;
	}
	return false;
}

void GPIOStateManager::update() {
	GPIOConfig config = cfgProvider.load();
	
	for(int i=0; i<=PIN_MAX; i++) {
		PinConfig pin = config.gpio[i];
		String displayName = pin.name; //  + String(" GPIO "); + String(i+'0')
		if(!isSafeToUse(i)) {
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