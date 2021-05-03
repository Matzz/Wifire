#pragma once

#include "../Codec.h"

class WiFiStationConfig {
public:
	bool enabled = false;
	String ssid;
	String password;
	String hostname;
	bool dhcp;
	IpAddress ip;
	IpAddress netmask;
	IpAddress gateway;
};

template<>
void Codec<WiFiStationConfig>::encode(JsonObject& json, const WiFiStationConfig &obj);

template<>
Either<String, WiFiStationConfig> Codec<WiFiStationConfig>::decode(JsonObject& json);