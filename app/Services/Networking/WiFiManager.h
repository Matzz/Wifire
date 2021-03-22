#pragma once

#include "../../Model/Networking/WiFiStationConfig.h"
#include "../../Model/Networking/WiFiApConfig.h"
#include "../../Utils/NonCopyable.h"
#include "../ConfigProvider.h"

class WiFiManager : private NonCopyable {
public:
	WiFiManager(ConfigProvider<WiFiStationConfig>& stationConfigProvider,
			    ConfigProvider<WiFiApConfig>& apConfigProvider);

	void refreshNetwork();
	void startTempStationMode();
	void stopTempStationMode();

protected:
	char tempStationUsersCnt = 0;
	ConfigProvider<WiFiStationConfig>& stationConfigProvider;
	ConfigProvider<WiFiApConfig>& apConfigProvider;
	WiFiApConfig currentApConfig;
	WiFiStationConfig currentStationConfig;

	void reloadConfiguration();

	void refreshAccessPoint();
	bool startAccessPoint();

	void refreshStation();
	bool connectStation();

	static void onAccessPointConnect(MacAddress mac, uint16_t aid);
	static void onAccessPointDisconnect(MacAddress mac, uint16_t aid);
	static void onStationConnect(const String& ssid, MacAddress bssid, uint8_t channel);
	static void onStationDisconnect(const String& ssid, MacAddress bssid, WifiDisconnectReason reason);
	static void onStationGotIP(IpAddress ip, IpAddress netmask, IpAddress gateway);
};
