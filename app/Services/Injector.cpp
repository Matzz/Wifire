#include "Injector.h"

Injector& Injector::getInstance() {
	static Injector injector;
	return injector;
}

template<typename T>
ConfigProvider<T>& Injector::getFileConfigProvider(const String &fileName) {
	static FileConfigProvider<T> instance(fileName);
	return instance;
}

template<typename T>
ConfigProvider<T>& Injector::getCachedFileConfigProvider(const String &fileName) {
	static CachedConfigProvider<T> instance = CachedConfigProvider(getFileConfigProvider<T>(fileName));
	return instance;
}

ConfigProvider<OtaConfig>& Injector::getOtaConfigProvider() {
	return getFileConfigProvider<OtaConfig>("config/ota.json");
}

ConfigProvider<WiFiStationConfig>& Injector::getWiFiStationConfigProvider() {
	return getFileConfigProvider<WiFiStationConfig>("config/wifi_station.json");
}

ConfigProvider<WiFiApConfig>& Injector::getWiFiApConfigProvider() {
	return getFileConfigProvider<WiFiApConfig>("config/wifi_ap.json");
}

ConfigProvider<GPIOConfig>& Injector::getGPIOConfigProvider() {
	return getCachedFileConfigProvider<GPIOConfig>("config/gpio.json");
}

ConfigProvider<UsersConfig>& Injector::getUsersConfigProvider() {
	return getFileConfigProvider<UsersConfig>("config/users.json");
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