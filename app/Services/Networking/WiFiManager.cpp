#include <SmingCore.h>
#include "WiFiManager.h"


WiFiManager::WiFiManager(ConfigProvider<WiFiStationConfig>& stationConfigProvider,
		ConfigProvider<WiFiApConfig>& apConfigProvider) :
		stationConfigProvider(stationConfigProvider), apConfigProvider(
				apConfigProvider) { }

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
		if(tempStationEnabled) {
			// We don't want to to turn off station while network scann is in progress.
			debug_i("Enabled station mode withut connecting.");
		} else {
			debug_i("Station mode disabled.");
			WifiStation.enable(false, false);
		}
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

/**
 * Usend only to list available networks. We don't connect to real network in this mode.
 * It should be disabled just after listing available networks.
 **/
void WiFiManager::startTempStationMode() {
	if(!WifiStation.isEnabled()) {
		// const String& ssid, const String& password, bool autoConnectOnStartup, bool save
		WifiStation.config(String::empty, String::empty, false, false);
		WifiStation.enable(true, false);
		tempStationEnabled = true;
	} else {
		// Started anyway. Nothing to do
	}
}

void WiFiManager::stopTempStationMode() {
	if(tempStationEnabled) {
		tempStationEnabled = false;
		WifiStation.enable(false, false);
	}
}

void onAccessPointConnect(MacAddress mac, uint16_t aid) {
	debug_i("AccessPointConnect mac: %s, aid: %d", mac.toString().c_str(), aid);
}

void onAccessPointDisconnect(MacAddress mac, uint16_t aid) {
	debug_i("AccessPointDisconnect mac: %s, aid: %d", mac.toString().c_str(), aid);
}

bool WiFiManager::startAccessPoint(WiFiApConfig& config) {
	WifiEvents.onAccessPointConnect(onAccessPointConnect);
	WifiEvents.onAccessPointDisconnect(onAccessPointDisconnect);

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

void onStationConnect(const String& ssid, MacAddress bssid, uint8_t channel)
{
	debug_i("Station connected to SSID: %s, MAC: %s, channel: %d.",
		ssid.c_str(),
		bssid.toString().c_str(),
		channel);
}

void onStationDisconnect(const String& ssid, MacAddress bssid, WifiDisconnectReason reason)
{
	debug_i("Station disconnected from SSID: %s, MAC: %s, reason: %s.",
		ssid,
		bssid.toString().c_str(),
		WifiEvents.getDisconnectReasonDesc(reason).c_str());
}

void onStationGotIP(IpAddress ip, IpAddress netmask, IpAddress gateway) {
	debug_i("Station got IP. IP: %s, Netmask: %s, Gateway: %s.",
		ip.toString().c_str(),
		netmask.toString().c_str(),
		gateway.toString().c_str());

}

bool WiFiManager::connectStation(WiFiStationConfig& config) {

	WifiEvents.onStationConnect(onStationConnect);
	WifiEvents.onStationDisconnect(onStationDisconnect);
	WifiEvents.onStationGotIP(onStationGotIP);

	if(config.hostname.length()>0) {
		WifiStation.setHostname(config.hostname);
	}
	if (!config.ip.isNull()) {
		WifiStation.setIP(config.ip, config.netmask, config.gateway);
	} else {
		debug_i("Station DHCP enabled");
		WifiStation.enableDHCP(true);
	}
	bool status =  WifiStation.config(config.ssid, config.password, false, true);
	if (!status) {
		return false;
	}
	WifiStation.enable(true, false);
	return WifiStation.connect();
}
