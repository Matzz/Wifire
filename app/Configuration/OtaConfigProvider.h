#pragma once

#include <SmingCore.h>
#include "FileConfig.h"

class OtaConfig {
public:
	String romUrl;
	String spiffUrl;
};

class OtaConfigProvider: FileConfig {
public:
	OtaConfigProvider(String fileName = "config/ota.json");
	OtaConfig load();
	void save(OtaConfig cfg);
};
