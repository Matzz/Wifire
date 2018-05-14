#ifndef APP_CONFIGURATION_OTACONFIGPROVIDER_H_
#define APP_CONFIGURATION_OTACONFIGPROVIDER_H_
#include <SmingCore/SmingCore.h>
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

#endif
