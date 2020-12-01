#pragma once

#include <SmingCore.h>
#include "../FileConfig.h"

class WiFiApConfig {
public:
	bool enabled = true;
	String ssid;
	String password;
	WifiAuthMode authMode;
	IPAddress ip;
	bool hidden = false;
	int channel = 7;
	int beaconInterval = 200;
};

class WiFiApConfigProvider: FileConfig {
public:
	WiFiApConfigProvider(String fileName = "config/wifi_ap.json");
	WiFiApConfig load();
	void save(WiFiApConfig cfg);
};
