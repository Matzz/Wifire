#include "Injector.h"

int atexit(void (*func)()) { return 0; }


Injector& Injector::getInstance() {
	static Injector injector;
	return injector;
}

OtaConfigProvider& Injector::getOtaConfigProvider() {
	static OtaConfigProvider otaConfigProvider;
	return otaConfigProvider;
}

WiFiStationConfigProvider& Injector::getWiFiStationConfigProvider() {
	static WiFiStationConfigProvider wiFiStationConfigProvider;
	return wiFiStationConfigProvider;
}

WiFiApConfigProvider& Injector::getWiFiApConfigProvider() {
	static WiFiApConfigProvider wiFiApConfigProvider;
	return wiFiApConfigProvider;
}

GPIOConfigProvider& Injector::getGPIOConfigProvider() {
	static GPIOConfigProvider gpioConfigProvider;
	return gpioConfigProvider;
}

UsersConfigProvider& Injector::getUsersConfigProvider() {
	static UsersConfigProvider usersConfigProvider;
	return usersConfigProvider;
}

UserSessionManager& Injector::getUserSessionManager() {
	static UserSessionManager userSessionManager(getUsersConfigProvider());
	return userSessionManager;
}

OtaUpdater& Injector::getOtaUpdater() {
	static OtaUpdater otaUpdater(spiffsAddresses, getOtaConfigProvider());
	return otaUpdater;
}

WiFiManager& Injector::getWiFiManager() {
	static WiFiManager wiFiManager(
		getWiFiStationConfigProvider(),
		getWiFiApConfigProvider());
	return wiFiManager;
}

WebServer& Injector::getWebServer() {
	static WebServer webServer(80);
	return webServer;
}

LazyFtpServer& Injector::getFtpServer() {
	static LazyFtpServer lazyFtpServer(ftpUser, ftpPassword, ftpPort);
	return lazyFtpServer;
}

GPIOStateManager& Injector::getGPIOStateManager() {
	GPIOConfigProvider& configProvider = getGPIOConfigProvider();
	static GPIOStateManager gpioStateManager(configProvider);
	return gpioStateManager;
}