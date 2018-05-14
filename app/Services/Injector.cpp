#include "Injector.h"

int atexit(void (*func)()) { return 0; }

Injector::Injector() { }

Injector& Injector::getInstance() {
	static Injector injector;
	return injector;
}

OtaConfigProvider& Injector::getOtaConfigProvider() {
	if (otaConfigProvider == NULL) {
		otaConfigProvider = new OtaConfigProvider();
	}
	return *otaConfigProvider;
}

WiFiStationConfigProvider& Injector::getWiFiStationConfigProvider() {
	if (wiFiStationConfigProvider == NULL) {
		wiFiStationConfigProvider = new WiFiStationConfigProvider();
	}
	return *wiFiStationConfigProvider;
}

WiFiApConfigProvider& Injector::getWiFiApConfigProvider() {
	if (wiFiApConfigProvider == NULL) {
		wiFiApConfigProvider = new WiFiApConfigProvider();
	}
	return *wiFiApConfigProvider;
}

OtaUpdater& Injector::getOtaUpdater() {
	if (otaUpdater == NULL) {
		otaUpdater = new OtaUpdater(spiffsAddresses, getOtaConfigProvider());
	}
	return *otaUpdater;
}

WiFiManager& Injector::getWiFiManager() {
	if (wiFiManager == NULL) {
		wiFiManager = new WiFiManager(getWiFiStationConfigProvider(),
				getWiFiApConfigProvider());
	}
	return *wiFiManager;
}

WebServer& Injector::getWebServer() {
	if (webServer == NULL) {
		webServer = new WebServer();
	}
	return *webServer;
}

FtpServer& Injector::getFtpServer() {
	if (ftpServer == NULL) {
		ftpServer = new FtpServer(ftpUser, ftpPassword, ftpPort);
	}
	return *ftpServer;
}
