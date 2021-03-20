#pragma once

#include <SmingCore.h>
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
		json["hostname"] = obj.hostname;
		json["ip"] = obj.ip.isNull() ? "" : obj.ip.toString();
		json["netmask"] = obj.netmask.isNull() ? "" : obj.netmask.toString();
		json["gateway"] = obj.gateway.isNull() ? "" : obj.gateway.toString();
	}

	Either<String, WiFiStationConfig> decode(JsonObject& json) {
		auto emptyIp = IpAddress("");
		WiFiStationConfig obj;
		if(json.isNull()) {
			obj.enabled = false;
			obj.ssid = String::empty;
			obj.password = String::empty;
			obj.hostname = String::empty;
			obj.ip = emptyIp;
			obj.netmask = emptyIp;
			obj.gateway = emptyIp;
		} else {
			obj.enabled = CodecHelpers::getOrElse(json, "enabled", false);
			obj.ssid = CodecHelpers::getOrElse(json, "ssid", "");
			obj.password = CodecHelpers::getOrElse(json, "password", "");
			String defaultHostname = "Wifire_" + String(system_get_chip_id(), 10);
			obj.hostname = CodecHelpers::getOrElse(json, "hostname", defaultHostname);
			obj.ip = CodecHelpers::getIp(json, "ip", emptyIp);
			obj.netmask = CodecHelpers::getIp(json, "netmask", emptyIp);
			obj.gateway = CodecHelpers::getIp(json, "gateway", emptyIp);
		}
		return obj;
	}
};