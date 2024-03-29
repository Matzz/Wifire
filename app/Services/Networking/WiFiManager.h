#pragma once

#include "../../Model/Networking/WiFiStationConfig.h"
#include "../../Model/Networking/WiFiApConfig.h"
#include "../../Utils/NonCopyable.h"
#include "../ConfigProvider.h"

class WiFiManager : private NonCopyable {
public:
	WiFiManager(ConfigProvider<WiFiStationConfig>& stationConfigProvider,
			    ConfigProvider<WiFiApConfig>& apConfigProvider);

	void initNetwork();
	void startNetwork();
	void startTempStationMode();
	void stopTempStationMode();

private:
	char tempStationUsersCnt = 0;
	ConfigProvider<WiFiStationConfig>& stationConfigProvider;
	ConfigProvider<WiFiApConfig>& apConfigProvider;

	const WiFiApConfig loadApConfig();
	const WiFiStationConfig loadStationConfig();

	void initAccessPoint();
	void startAccessPoint();

	void initStation();
	void startStation();

	static void onAccessPointConnect(MacAddress mac, uint16_t aid);
	static void onAccessPointDisconnect(MacAddress mac, uint16_t aid);
	static void onStationConnect(const String& ssid, MacAddress bssid, uint8_t channel);
	static void onStationDisconnect(const String& ssid, MacAddress bssid, WifiDisconnectReason reason);
	static void onStationGotIP(IpAddress ip, IpAddress netmask, IpAddress gateway);
};
