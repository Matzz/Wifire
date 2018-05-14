#ifndef APP_SERVICES_WIFIMANAGER_H_
#define APP_SERVICES_WIFIMANAGER_H_

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

#endif
