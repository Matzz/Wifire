#include <SmingCore.h>
#include "WiFiManager.h"


WiFiManager::WiFiManager(ConfigProvider<WiFiStationConfig>& stationConfigProvider,
		ConfigProvider<WiFiApConfig>& apConfigProvider) :
		stationConfigProvider(stationConfigProvider), apConfigProvider(
				apConfigProvider) {
}

void WiFiManager::startNetwork() {
	auto stCfgOrError = stationConfigProvider.load();
	if(stCfgOrError.isLeft()) {
		debug_w("Station config error: %s", stCfgOrError.getIfLeft()->c_str());
		return;
	}
	auto stCfg = *stCfgOrError.getIfRight();

	auto apCfgOrError = apConfigProvider.load();
	if(apCfgOrError.isLeft()) {
		debug_w("Access point config error: %s", apCfgOrError.getIfLeft()->c_str());
		return;
	}
	auto apCfg = *apCfgOrError.getIfRight();
	

	bool connectedToStation = false;
	bool apStarted = false;

	if(stCfg.enabled) {
		debug_i("Station mode enabled.");
		if(connectStation(stCfg)) {
			debug_i("Connected to station.");
			connectedToStation = true;
		} else {
			debug_w("Cannot connect to station.");
		}
	} else {
		WifiStation.enable(false, false);
		debug_i("Station mode disabled.");
	}
	if(apCfg.enabled) {
		if(startAccessPoint(apCfg)) {
			debug_i("AP created.");
			apStarted = true;
		} else {
			debug_w("Cannot create AP.");
		}
	} else {
		WifiAccessPoint.enable(false, false);
		debug_i("AP mode disabled.");
	}
}


bool WiFiManager::startAccessPoint(WiFiApConfig& config) {

	debug_i("Starting network %s", config.ssid.c_str());
	WifiAccessPoint.enable(true, false);
	bool configStatus = WifiAccessPoint.config(config.ssid, config.password,
			config.authMode, config.hidden, config.channel,
			config.beaconInterval);
	if (!configStatus) {
		debug_w("Setting AP config failed.");
	}

	IpAddress oldIp = WifiAccessPoint.getIP();
	if (!(oldIp==config.ip)) {
		debug_i("Setting new ip %s. Old ip %s\n", config.ip.toString().c_str(), oldIp.toString().c_str());
		if (!WifiAccessPoint.setIP(config.ip)) {
			debug_w("Setting ip failed.");
		}
	} else {
		debug_i("Keeping old ip %s\n", oldIp.toString().c_str());
	}

	return true;
}

bool WiFiManager::connectStation(WiFiStationConfig& config) {
	if (!config.ip.isNull()) {
		WifiStation.setIP(config.ip, config.netmask, config.gateway);
	} else {
		WifiStation.enableDHCP(true);
	}
	bool status =  WifiStation.config(config.ssid, config.password, false, true);
	if (!status) {
		return false;
	}
	WifiStation.enable(true, false);
	return WifiStation.connect();
}
