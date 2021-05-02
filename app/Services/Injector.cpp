#include "Injector.h"

Injector& Injector::getInstance() {
	static Injector injector;
	return injector;
}

ConfigProvider<OtaConfig>& Injector::getOtaConfigProvider() {
	static FileConfigProvider<OtaConfig> otaConfigProvider("config/ota.json", Codec<OtaConfig>::getInstance());
	return otaConfigProvider;
}

ConfigProvider<WiFiStationConfig>& Injector::getWiFiStationConfigProvider() {
	static FileConfigProvider<WiFiStationConfig> wiFiStationConfigProvider(
		"config/wifi_station.json",
		Codec<WiFiStationConfig>::getInstance());
	return wiFiStationConfigProvider;
}

ConfigProvider<WiFiApConfig>& Injector::getWiFiApConfigProvider() {
	static auto wiFiApConfigProvider = FileConfigProvider<WiFiApConfig>( "config/wifi_ap.json", Codec<WiFiApConfig>::getInstance());
	return wiFiApConfigProvider;
}

ConfigProvider<GPIOConfig>& Injector::getGPIOConfigProvider() {
	static FileConfigProvider<GPIOConfig> fCfg("config/gpio.json", Codec<GPIOConfig>::getInstance());
	static CachedConfigProvider<GPIOConfig> gpioConfigProvider = CachedConfigProvider(fCfg);
	return gpioConfigProvider;
}

ConfigProvider<UsersConfig>& Injector::getUsersConfigProvider() {
	static FileConfigProvider<UsersConfig> usersConfigProvider("config/users.json", Codec<UsersConfig>::getInstance());
	return usersConfigProvider;
}

ConfigProvider<Vector<Session>>& Injector::getSessionsProvider() {
	static SessionsConfigProvider sessionsProvider("config/sessions.json", getUsersConfigProvider());
	return sessionsProvider;
}

UserSessionManager& Injector::getUserSessionManager() {
	static UserSessionManager userSessionManager(getUsersConfigProvider(), getSessionsProvider());
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

AvaiableNetworksProvider& Injector::getAvaiableNetworksProvider() {
	static AvaiableNetworksProvider avaiableNetworksProvider(
		getWiFiManager()
	);
	return avaiableNetworksProvider;
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