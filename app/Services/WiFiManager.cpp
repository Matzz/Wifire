#include "WiFiManager.h"
#include <SmingCore.h>


WiFiManager::WiFiManager(WiFiStationConfigProvider& stationConfigProvider,
		WiFiApConfigProvider& apConfigProvider) :
		stationConfigProvider(stationConfigProvider), apConfigProvider(
				apConfigProvider) {

}

void WiFiManager::startNetwork() {
	WiFiStationConfig stCfg = stationConfigProvider.load();
	WiFiApConfig apCfg = apConfigProvider.load();
	bool connectedToStation = false;
	bool apStarted = false;

	if(stCfg.enabled) {
		Serial.println("Station mode enabled.");
		if(connectStation(stCfg)) {
			Serial.println("Connected to station.");
			connectedToStation = true;
		} else {
			Serial.println("Cannot connect to station.");
		}
	} else {
		Serial.println("Station mode disabled.");
	}
	if(apCfg.enabled) {
		if(startAccessPoint(apCfg)) {
			Serial.println("AP created.");
			apStarted = true;
		} else {
			Serial.println("Cannot create AP.");
		}
	} else {
		Serial.println("AP mode disabled.");

	}
}


bool WiFiManager::startAccessPoint(WiFiApConfig& config) {

	Serial.println("Starting network " + config.ssid);
	bool configStatus = WifiAccessPoint.config(config.ssid, config.password,
			config.authMode, config.hidden, config.channel,
			config.beaconInterval);
	if (!configStatus) {
		Serial.println("Setting AP config failed.");
	}
	WifiAccessPoint.enable(true);

	IPAddress oldIp = WifiAccessPoint.getIP();
	if (!(oldIp==config.ip)) {
		Serial.printf("Setting new ip %s. Old ip %s\n", config.ip.toString().c_str(), oldIp.toString().c_str());
		if (!WifiAccessPoint.setIP(config.ip)) {
			Serial.println("Setting ip failed.");
		}
	} else {
		Serial.printf("Keeping old ip %s\n", oldIp.toString().c_str());
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
	WifiStation.enable(true);
	return WifiStation.connect();
}
