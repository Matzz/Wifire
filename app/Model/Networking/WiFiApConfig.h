#pragma once

#include "../Codec.h"

class WiFiApConfig {
public:
	bool enabled = true;
	String ssid;
	String password;
	WifiAuthMode authMode;
	IpAddress ip;
	bool hidden = false;
	unsigned int channel = 7;
	unsigned int beaconInterval = 200;
};

template<>
void Codec<WiFiApConfig>::encode(JsonObject& json, const WiFiApConfig &obj);

template<>
Either<String, WiFiApConfig> Codec<WiFiApConfig>::decode(JsonObject& json);