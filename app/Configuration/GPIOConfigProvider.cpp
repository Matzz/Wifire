#include "GPIOConfigProvider.h"

#include <SmingCore.h>
#include "FileConfig.h"


GPIOConfigProvider::GPIOConfigProvider(String fileName) :
			FileConfig(fileName) { }



GPIOConfig GPIOConfigProvider::jsonToConfig(JsonObject& doc) {
	GPIOConfig cfg;
	JsonArray gpioArr = doc["gpio"].as<JsonArray>();
	int gpioArrSize = gpioArr.size();
	for(int i=0; i<=PIN_MAX; i++) {
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
	for(int i=0; i<=PIN_MAX; i++) {
		JsonObject pinObj = gpioArr.createNestedObject();
		pinObj["name"] = config.gpio[i].name;
		pinObj["isInput"] = config.gpio[i].isInput;
		pinObj["pull"] = config.gpio[i].pull;
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
