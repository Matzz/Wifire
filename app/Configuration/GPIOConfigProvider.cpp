#include "GPIOConfigProvider.h"

#include <SmingCore.h>
#include "FileConfig.h"

const unsigned int GPIOConfig::safe_pins[] = {2,3,4,5,13,14,15,16};

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
bool GPIOConfig::isSafeToUse(unsigned int pin) {
	return std::find(std::begin(GPIOConfig::safe_pins), std::end(GPIOConfig::safe_pins), pin) != std::end(GPIOConfig::safe_pins);
}

GPIOConfigProvider::GPIOConfigProvider(String fileName) :
			FileConfig(fileName) { }


GPIOConfig GPIOConfigProvider::jsonToConfig(JsonObject& doc) {
	debug_i("Loading GPIO configuration.");
	GPIOConfig cfg;
	JsonArray gpioArr = doc["gpio"].as<JsonArray>();
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

void GPIOConfigProvider::configToJson(GPIOConfig& config, JsonObject& doc) {
	JsonArray gpioArr = doc.createNestedArray("gpio");
	for(int i=0; i<=GPIOConfig::max_pin; i++) {
		JsonObject pinObj = gpioArr.createNestedObject();
		pinObj["name"] = config.gpio[i].name;
		bool isSafe = GPIOConfig::isSafeToUse(i);
		pinObj["isInput"] = isSafe ? config.gpio[i].isInput : false;
		pinObj["pull"] = isSafe ? config.gpio[i].pull : false;
	}
}

GPIOConfig GPIOConfigProvider::load() {
	debug_i("Loading GPIO config.");

	StaticJsonDocument<JSON_MAX_SIZE> doc;
	loadJsonObject(doc);
	JsonObject obj = doc.as<JsonObject>();
	return jsonToConfig(obj);
}

void GPIOConfigProvider::save(GPIOConfig config) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	JsonObject obj = doc.to<JsonObject>();
	configToJson(config, obj);
	saveJsonObject(doc);
}
