#include "GPIOConfigProvider.h"

#include <SmingCore.h>
#include "FileConfig.h"


GPIOConfigProvider::GPIOConfigProvider(String fileName) :
			FileConfig(fileName) { }

GPIOConfig GPIOConfigProvider::load() {
	Serial.println("Loading GPIO config.");
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	loadJsonObject(doc);
	GPIOConfig cfg;
	Serial.println("Dec");
	JsonArray modesArr = doc["modes"].as<JsonArray>();
	JsonArray pullUpsArr = doc["pullUps"].as<JsonArray>();
	JsonArray namesArr = doc["names"].as<JsonArray>();
	Serial.println("Dec end");
	int modesArrSize = modesArr.size();
	int pullUpsArrSize = pullUpsArr.size();
	int namesArrSize = namesArr.size();
	for(int i=0; i<PIN_MAX; i++) {
		cfg.modes[i] = i<modesArrSize ? modesArr[i] : INPUT;
		cfg.pullUps[i] = i<pullUpsArrSize ? modesArr[i] : false;
		cfg.names[i] = i<namesArrSize ? namesArr[i].as<String>() : "GPIO_"+String(i);

	}
	Serial.println("END");
	return cfg;
}

void GPIOConfigProvider::save(GPIOConfig cfg) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	saveJsonObject(doc);
}
