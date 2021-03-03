#pragma once

#include <SmingCore.h>
#include "../Codec.h"

class WiFiStationConfig {
public:
	bool enabled = false;
	String ssid;
	String password;
	IpAddress ip;
	IpAddress netmask;
	IpAddress gateway;
};

template<> class Codec<WiFiStationConfig> {
	public:
	static Codec<WiFiStationConfig>& getInstance() {
		static Codec<WiFiStationConfig> instance;
		return instance;
	}

	void encode(JsonObject& json, WiFiStationConfig obj) {
		json["enabled"] = obj.enabled;
		json["ssid"] = obj.ssid;
		json["password"] = obj.password;
		json["ip"] = obj.ip.isNull() ? "" : obj.ip.toString();
		json["netmask"] = obj.netmask.isNull() ? "" : obj.netmask.toString();
		json["gateway"] = obj.gateway.isNull() ? "" : obj.gateway.toString();
	}

	Either<String, WiFiStationConfig> decode(JsonObject& json) {
		WiFiStationConfig obj;
		obj.enabled = CodecHelpers::getOrElse(json, "enabled", false);
		obj.ssid = CodecHelpers::getOrElse(json, "ssid", "");
		obj.password = CodecHelpers::getOrElse(json, "password", "");
		obj.ip = CodecHelpers::getIp(json, "ip", IpAddress(""));
		obj.netmask = CodecHelpers::getIp(json, "netmask", IpAddress(""));
		obj.gateway = CodecHelpers::getIp(json, "gateway", IpAddress(""));
		return obj;
	}
};