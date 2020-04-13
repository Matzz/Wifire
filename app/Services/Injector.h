#pragma once

#include <SmingCore.h>
#include "OtaUpdater.h"
#include "../Configuration/OtaConfigProvider.h"
#include "../Configuration/WiFiApConfigProvider.h"
#include "../Configuration/WiFiStationConfigProvider.h"
#include "../Configuration/GPIOConfigProvider.h"
#include "LazyFtpServer.h"
#include "WiFiManager.h"
#include "WebServer.h"

class Injector {
private:
	Injector();

	void operator=(Injector const&);

	OtaConfigProvider* otaConfigProvider;
	WiFiStationConfigProvider* wiFiStationConfigProvider;
	WiFiApConfigProvider* wiFiApConfigProvider;
	GPIOConfigProvider* gpioConfigProvider;
	OtaUpdater* otaUpdater;
	WiFiManager* wiFiManager;
	WebServer* webServer;
	LazyFtpServer* ftpServer;

public:
	static Injector& getInstance();

	const int spiffsAddresses[2] = { RBOOT_SPIFFS_0, RBOOT_SPIFFS_1 };
	const int spiffsSize = SPIFF_SIZE;

	const int httpPort = 80;

	const int ftpPort = 21;
	const String ftpUser = "wifire";
	const String ftpPassword = "wifirer";

	OtaConfigProvider& getOtaConfigProvider();
	WiFiStationConfigProvider& getWiFiStationConfigProvider();
	WiFiApConfigProvider& getWiFiApConfigProvider();
	GPIOConfigProvider& getGPIOConfigProvider();
	OtaUpdater& getOtaUpdater();
	WiFiManager& getWiFiManager();
	WebServer& getWebServer();
	LazyFtpServer& getFtpServer();
};
