//#include "GPIOConfigProvider.h"
//
//#include <array>
//#include <SmingCore.h>
//#include "FileConfig.h"
//std::array::fill();
//
//
//GPIOConfigProvider::GPIOConfigProvider(String fileName) :
//			FileConfig(fileName) { }
//
//GPIOConfig GPIOConfigProvider::load() {
//	Serial.println("Loading GPIO config.");
//	StaticJsonBuffer<1024> jsonBuffer;
//	JsonObject& json = loadJsonObject(jsonBuffer);
//	GPIOConfig cfg;
//	json.ar
//	cfg.pinModes = getOrElse(json, "pinModes", "");
//	cfg.names = getOrElse(json, "pinNames", DEFAULT_GPIO_NAMES);
//	cfg.pullUp = getOrElse(json, "pinNames", DEFAULT_GPIO_NAMES);
//	return cfg;
//}
//
//void GPIOConfigProvider::save(GPIOConfig cfg) {
//	DynamicJsonBuffer jsonBuffer;
//	JsonObject& root = jsonBuffer.createObject();
////	root["romUrl"] = cfg.romUrl.c_str();
////	root["spiffUrl"] = cfg.spiffUrl.c_str();
//	saveJsonObject(root);
//}
