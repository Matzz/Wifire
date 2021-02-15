#pragma once

#include "../../Model/Networking/WiFiStationConfig.h"
#include "../../Model/Networking/WiFiApConfig.h"
#include "../ConfigProvider.h"

class WiFiManager {
public:
	WiFiManager(ConfigProvider<WiFiStationConfig>& stationConfigProvider,
			ConfigProvider<WiFiApConfig>& apConfigProvider);

	void startNetwork();

protected:
	ConfigProvider<WiFiStationConfig>& stationConfigProvider;
	ConfigProvider<WiFiApConfig>& apConfigProvider;

	bool startAccessPoint(WiFiApConfig& config);

	bool connectStation(WiFiStationConfig& config);
};
