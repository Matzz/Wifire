#pragma once

#include <SmingCore.h>
#include "OtaUpdater.h"
#include "../Configuration/ConfigProvider.h"
#include "../Configuration/OtaConfig.h"
#include "../Configuration/GPIOConfig.h"
#include "../Configuration/Networking/WiFiApConfig.h"
#include "../Configuration/Networking/WiFiStationConfig.h"
#include "../Configuration/Users/UsersConfig.h"
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

	ConfigProvider<OtaConfig>& getOtaConfigProvider();
	ConfigProvider<WiFiStationConfig>& getWiFiStationConfigProvider();
	ConfigProvider<WiFiApConfig>& getWiFiApConfigProvider();
	ConfigProvider<GPIOConfig>& getGPIOConfigProvider();
	ConfigProvider<UsersConfig>& getUsersConfigProvider();
	UserSessionManager& getUserSessionManager();
	OtaUpdater& getOtaUpdater();
	WiFiManager& getWiFiManager();
	WebServer& getWebServer();
	LazyFtpServer& getFtpServer();
	GPIOStateManager& getGPIOStateManager();
};
