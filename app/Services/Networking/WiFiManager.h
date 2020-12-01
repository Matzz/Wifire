#pragma once

#include "../../Configuration/Networking/WiFiStationConfigProvider.h"
#include "../../Configuration/Networking/WiFiApConfigProvider.h"

class WiFiManager {
public:
	WiFiManager(WiFiStationConfigProvider& stationConfigProvider,
			WiFiApConfigProvider& apConfigProvider);

	void startNetwork();

protected:
	WiFiStationConfigProvider& stationConfigProvider;
	WiFiApConfigProvider& apConfigProvider;

	bool startAccessPoint(WiFiApConfig& config);

	bool connectStation(WiFiStationConfig& config);
};