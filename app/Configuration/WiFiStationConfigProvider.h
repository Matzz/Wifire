#pragma once

#include <SmingCore.h>
#include "FileConfig.h"

class WiFiStationConfig {
public:
	bool enabled = false;
	String ssid;
	String password;
	IPAddress ip;
	IPAddress netmask;
	IPAddress gateway;
};

class WiFiStationConfigProvider: FileConfig {
public:
	WiFiStationConfigProvider(String fileName = "config/wifi_station.json");

	WiFiStationConfig load();

	void save(WiFiStationConfig cfg);
};
