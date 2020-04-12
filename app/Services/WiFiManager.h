#pragma once

#include "../Configuration/WiFiStationConfigProvider.h"
#include "../Configuration/WiFiApConfigProvider.h"

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
