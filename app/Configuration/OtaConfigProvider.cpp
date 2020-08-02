#include "OtaConfigProvider.h"

#include <SmingCore.h>
#include "FileConfig.h"

OtaConfigProvider::OtaConfigProvider(String fileName) :
			FileConfig(fileName) { }

OtaConfig OtaConfigProvider::load() {
	debug_i("Loading OTA config.");
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	loadJsonObject(doc);
	OtaConfig cfg;
	cfg.romUrl = getOrElse(doc, "romUrl", "");
	cfg.spiffUrl = getOrElse(doc, "spiffUrl", "");
	return cfg;
}

void OtaConfigProvider::save(OtaConfig cfg) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	doc["romUrl"] = cfg.romUrl.c_str();
	doc["spiffUrl"] = cfg.spiffUrl.c_str();
	saveJsonObject(doc);
}
