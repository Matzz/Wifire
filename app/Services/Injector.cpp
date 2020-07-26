#include "Injector.h"

int atexit(void (*func)()) { return 0; }


Injector& Injector::getInstance() {
	static Injector injector;
	return injector;
}

OtaConfigProvider& Injector::getOtaConfigProvider() {
	static auto otaConfigProvider = OtaConfigProvider();
	return otaConfigProvider;
}

WiFiStationConfigProvider& Injector::getWiFiStationConfigProvider() {
	static auto wiFiStationConfigProvider = WiFiStationConfigProvider();
	return wiFiStationConfigProvider;
}

WiFiApConfigProvider& Injector::getWiFiApConfigProvider() {
	static auto wiFiApConfigProvider = WiFiApConfigProvider();
	return wiFiApConfigProvider;
}

GPIOConfigProvider& Injector::getGPIOConfigProvider() {
	static auto gpioConfigProvider = GPIOConfigProvider();
	return gpioConfigProvider;
}

OtaUpdater& Injector::getOtaUpdater() {
	static auto otaUpdater = OtaUpdater(spiffsAddresses, getOtaConfigProvider());
	return otaUpdater;
}

WiFiManager& Injector::getWiFiManager() {
	static auto wiFiManager = WiFiManager(
		getWiFiStationConfigProvider(),
		getWiFiApConfigProvider());
	return wiFiManager;
}

WebServer& Injector::getWebServer() {
	static auto webServer = WebServer();
	return webServer;
}

LazyFtpServer& Injector::getFtpServer() {
	static auto lazyFtpServer = LazyFtpServer(ftpUser, ftpPassword, ftpPort);
	return lazyFtpServer;
}

GPIOStateManager& Injector::getGPIOStateManager() {
	static auto gpioStateManager = GPIOStateManager(getGPIOConfigProvider());
	return gpioStateManager;
}