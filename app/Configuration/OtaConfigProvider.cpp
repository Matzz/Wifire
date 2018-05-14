#include "OtaConfigProvider.h"

#include <SmingCore/SmingCore.h>
#include "FileConfig.h"

OtaConfigProvider::OtaConfigProvider(String fileName) :
			FileConfig(fileName) { }

OtaConfig OtaConfigProvider::load() {
	Serial.println("Loading OTA config.");
	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject& json = loadJsonObject(jsonBuffer);
	OtaConfig cfg;
	cfg.romUrl = getOrElse(json, "romUrl", "");
	cfg.spiffUrl = getOrElse(json, "spiffUrl", "");
	return cfg;
}

void OtaConfigProvider::save(OtaConfig cfg) {
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["romUrl"] = cfg.romUrl.c_str();
	root["spiffUrl"] = cfg.spiffUrl.c_str();
	saveJsonObject(root);
}
