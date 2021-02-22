#include "Injector.h"

Injector& Injector::getInstance() {
	static Injector injector;
	return injector;
}

ConfigProvider<OtaConfig>& Injector::getOtaConfigProvider() {
	static ConfigProvider<OtaConfig> otaConfigProvider("config/ota.json", Codec<OtaConfig>::getInstance());
	return otaConfigProvider;
}

ConfigProvider<WiFiStationConfig>& Injector::getWiFiStationConfigProvider() {
	static ConfigProvider<WiFiStationConfig> wiFiStationConfigProvider(
		"config/wifi_station.json",
		Codec<WiFiStationConfig>::getInstance());
	return wiFiStationConfigProvider;
}

ConfigProvider<WiFiApConfig>& Injector::getWiFiApConfigProvider() {
	static ConfigProvider<WiFiApConfig> wiFiApConfigProvider(
		"config/wifi_ap.json",
		Codec<WiFiApConfig>::getInstance());
	return wiFiApConfigProvider;
}

ConfigProvider<GPIOConfig>& Injector::getGPIOConfigProvider() {
	static ConfigProvider<GPIOConfig> gpioConfigProvider("config/gpio.json", Codec<GPIOConfig>::getInstance());
	return gpioConfigProvider;
}

ConfigProvider<UsersConfig>& Injector::getUsersConfigProvider() {
	static ConfigProvider<UsersConfig> usersConfigProvider("config/users.json", Codec<UsersConfig>::getInstance());
	return usersConfigProvider;
}

UserSessionManager& Injector::getUserSessionManager() {
	static UserSessionManager userSessionManager(getUsersConfigProvider());
	return userSessionManager;
}

OtaUpdater& Injector::getOtaUpdater() {
	static OtaUpdater otaUpdater(getOtaConfigProvider());
	return otaUpdater;
}

WiFiManager& Injector::getWiFiManager() {
	static WiFiManager wiFiManager(
		getWiFiStationConfigProvider(),
		getWiFiApConfigProvider());
	return wiFiManager;
}

WebServer& Injector::getWebServer() {
	static WebServer webServer(80, getUserSessionManager());
	return webServer;
}

LazyFtpServer& Injector::getFtpServer() {
	static LazyFtpServer lazyFtpServer(ftpUser, ftpPassword, ftpPort);
	return lazyFtpServer;
}

GPIOStateManager& Injector::getGPIOStateManager() {
	static GPIOStateManager gpioStateManager(getGPIOConfigProvider());
	return gpioStateManager;
}