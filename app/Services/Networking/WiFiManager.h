#pragma once

#include "../../Model/Networking/WiFiStationConfig.h"
#include "../../Model/Networking/WiFiApConfig.h"
#include "../../Utils/NonCopyable.h"
#include "../ConfigProvider.h"

class WiFiManager : private NonCopyable {
public:
	WiFiManager(ConfigProvider<WiFiStationConfig>& stationConfigProvider,
			    ConfigProvider<WiFiApConfig>& apConfigProvider);

	void startNetwork();
	void startTempStationMode();
	void stopTempStationMode();

protected:
	bool tempStationEnabled = false;
	ConfigProvider<WiFiStationConfig>& stationConfigProvider;
	ConfigProvider<WiFiApConfig>& apConfigProvider;

	bool startAccessPoint(WiFiApConfig& config);

	bool connectStation(WiFiStationConfig& config);
};
