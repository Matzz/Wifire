#pragma once

#include "../../Configuration/Networking/WiFiStationConfig.h"
#include "../../Configuration/Networking/WiFiApConfig.h"
#include "../../Configuration/ConfigProvider.h"

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
