#pragma once

#include <SmingCore.h>
#include "OtaUpdater.h"
#include "../Configuration/OtaConfigProvider.h"
#include "../Configuration/GPIOConfigProvider.h"
#include "../Configuration/Networking/WiFiApConfigProvider.h"
#include "../Configuration/Networking/WiFiStationConfigProvider.h"
#include "../Configuration/Users/UsersConfigProvider.h"
#include "../Services/UserSessionManager.h"
#include "LazyFtpServer.h"
#include "Networking/WiFiManager.h"
#include "WebServer.h"
#include "GPIOStateManager.h"

class Injector {
private:
	Injector()  {
		debug_i("Creating main Wifire module");
	};
	Injector(Injector const&);             // copy constructor is private
	Injector& operator=(Injector const&);  // assignment operator is private

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
	UsersConfigProvider& getUsersConfigProvider();
	UserSessionManager& getUserSessionManager();
	OtaUpdater& getOtaUpdater();
	WiFiManager& getWiFiManager();
	WebServer& getWebServer();
	LazyFtpServer& getFtpServer();
	GPIOStateManager& getGPIOStateManager();
};
