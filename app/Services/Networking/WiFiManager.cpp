#include <SmingCore.h>
#include "WiFiManager.h"


WiFiManager::WiFiManager(ConfigProvider<WiFiStationConfig>& stationConfigProvider,
	ConfigProvider<WiFiApConfig>& apConfigProvider) :
	stationConfigProvider(stationConfigProvider), apConfigProvider(apConfigProvider) {
	WifiEvents.onAccessPointConnect(onAccessPointConnect);
	WifiEvents.onAccessPointDisconnect(onAccessPointDisconnect);
	WifiEvents.onStationConnect(onStationConnect);
	WifiEvents.onStationDisconnect(onStationDisconnect);
	WifiEvents.onStationGotIP(onStationGotIP);
}


const WiFiApConfig WiFiManager::loadApConfig() {
	auto apCfgOrError = apConfigProvider.load();
	if(apCfgOrError.isLeft()) {
		debug_w("Access point config error: %s", apCfgOrError.getIfLeft()->c_str());
		return WiFiApConfig();
	} else {
		return *apCfgOrError.getIfRight();
	}
}


const WiFiStationConfig WiFiManager::loadStationConfig() {
	auto stCfgOrError = stationConfigProvider.load();
	if(stCfgOrError.isLeft()) {
		debug_w("Station config error: %s", stCfgOrError.getIfLeft()->c_str());
		return WiFiStationConfig();
	} else {
		return *stCfgOrError.getIfRight();
	}
}

void WiFiManager::initNetwork() {
	initStation();
	initAccessPoint();
}

void WiFiManager::startNetwork() {
	startStation();
	startAccessPoint();
}

void WiFiManager::initAccessPoint() {
	const WiFiApConfig& currentApConfig = loadApConfig();
	if(currentApConfig.enabled) {
		debug_i("AP mode enabled. Initializing.");
		IpAddress oldIp = WifiAccessPoint.getIP();
		if (!(oldIp == currentApConfig.ip)) {
			debug_i("Setting new AP IP %s. Old ip %s\n", currentApConfig.ip.toString().c_str(), oldIp.toString().c_str());
			if (!WifiAccessPoint.setIP(currentApConfig.ip)) {
				debug_w("Setting AP IP failed.");
			}
		} else {
			debug_i("Keeping old AP IP %s\n", oldIp.toString().c_str());
		}
	} else {
		debug_i("AP mode initializing disabled.");
	}
}

void WiFiManager::startAccessPoint() {
	const WiFiApConfig& currentApConfig = loadApConfig();
	if(currentApConfig.enabled) {
		debug_i("Starting AP network %s", currentApConfig.ssid.c_str());
		WifiAccessPoint.enable(true, false);
		bool configStatus = WifiAccessPoint.config(currentApConfig.ssid, currentApConfig.password,
				currentApConfig.authMode, currentApConfig.hidden, currentApConfig.channel,
				currentApConfig.beaconInterval);
		if (!configStatus) {
			debug_w("Setting AP config failed. Cannot create AP");
			return;
		} else {
			debug_i("AP created.");
			return;
		}
	} else {
		debug_i("AP mode disabled.");
		WifiAccessPoint.enable(false, false);
	}
}

void WiFiManager::initStation() {
	const WiFiStationConfig& currentStationConfig = loadStationConfig();
	if(currentStationConfig.enabled) {
		debug_i("Connectiong station network %s", currentStationConfig.ssid.c_str());
		if(currentStationConfig.hostname.length()>0) {
			debug_i("Station setting hostname to %s", currentStationConfig.hostname.c_str());
			WifiStation.setHostname(currentStationConfig.hostname);
		}
		if (!currentStationConfig.ip.isNull()) {
			debug_i("Station set Ip: %s, netmask %s, gateway %s",
				currentStationConfig.ip.toString(), 
				currentStationConfig.netmask.toString(), 
				currentStationConfig.gateway.toString()
				);
			WifiStation.setIP(currentStationConfig.ip, currentStationConfig.netmask, currentStationConfig.gateway);
		} else {
			debug_i("Station DHCP enabled");
			WifiStation.enableDHCP(true);
		}
	} else {
		debug_i("Station mode disabled.");
	}
}

void WiFiManager::startStation() {
	const WiFiStationConfig& currentStationConfig = loadStationConfig();
	if(currentStationConfig.enabled) {
		debug_i("Station mode enabled.");
		bool status =  WifiStation.config(currentStationConfig.ssid, currentStationConfig.password, false, false);
		if (!status) {
			debug_i("Station failed to set configuration.");
			return;
		}

		debug_i("Station invoking WifiStation.connect.");
		WifiStation.enable(true, false);
		if(WifiStation.connect()) {
			debug_i("Connected to station");
		} else {
			debug_i("Station connection failed.");
		}
	} else {
		#ifndef ARCH_HOST
		WifiStation.disconnect();
		#endif
		if(tempStationUsersCnt>0) {
			WifiStation.enable(true, false);
			debug_i("Enabled station mode withut connecting.");
		} else {
			WifiStation.enable(false, false);
			debug_i("Station mode disabled.");
		}
	}
}

/**
 * Usend only to list available networks. We don't connect to real network in this mode.
 * It should be disabled just after listing available networks.
 **/
void WiFiManager::startTempStationMode() {
	tempStationUsersCnt++;
	startStation();
}

void WiFiManager::stopTempStationMode() {
	tempStationUsersCnt--;
	startStation();
}


void WiFiManager::onAccessPointConnect(MacAddress mac, uint16_t aid) {
	debug_i("AccessPointConnect mac: %s, aid: %d", mac.toString().c_str(), aid);
}

void WiFiManager::onAccessPointDisconnect(MacAddress mac, uint16_t aid) {
	debug_i("AccessPointDisconnect mac: %s, aid: %d", mac.toString().c_str(), aid);
}

void WiFiManager::onStationConnect(const String& ssid, MacAddress bssid, uint8_t channel)
{
	debug_i("Station connected to SSID: %s, MAC: %s, channel: %d.",
		ssid.c_str(),
		bssid.toString().c_str(),
		channel);
}

void WiFiManager::onStationDisconnect(const String& ssid, MacAddress bssid, WifiDisconnectReason reason)
{
	debug_i("Station disconnected from SSID: %s, MAC: %s, reason: %s.",
		ssid,
		bssid.toString().c_str(),
		WifiEvents.getDisconnectReasonDesc(reason).c_str());
}

void WiFiManager::onStationGotIP(IpAddress ip, IpAddress netmask, IpAddress gateway) {
	debug_i("Station got IP. IP: %s, Netmask: %s, Gateway: %s.",
		ip.toString().c_str(),
		netmask.toString().c_str(),
		gateway.toString().c_str());
}