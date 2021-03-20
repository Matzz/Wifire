#pragma once

#include <SmingCore.h>
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


template<> class Codec<WiFiApConfig> {
	public:
        static Codec<WiFiApConfig>& getInstance() {
            static Codec<WiFiApConfig> instance;
            return instance;
        }

	void encode(JsonObject& json, WiFiApConfig obj) {
		json["enabled"] = obj.enabled;
		json["ssid"] = obj.ssid;
		json["password"] = obj.password;
		json["authMode"] = (int) obj.authMode;
		json["ip"] = obj.ip.isNull() ? "" : obj.ip.toString();
		json["hidden"] = obj.hidden;
		json["channel"] = obj.channel;
		json["beaconInterval"] = obj.beaconInterval;
	}

	Either<String, WiFiApConfig> decode(JsonObject& json) {
		WiFiApConfig obj;
		String defaultSsid = "Wifire_" + String(system_get_chip_id(), 10);
		if(json.isNull()) {
			obj.ssid = defaultSsid;
			obj.enabled = true;
			obj.password = "";
			obj.ip = IpAddress(192,168,1,1);
			obj.authMode = WifiAuthMode::AUTH_OPEN;
			obj.hidden = false;
		} else {
			obj.enabled = CodecHelpers::getOrElse(json, "enabled", false);
			obj.ssid = CodecHelpers::getOrElse(json, "ssid", defaultSsid);
			obj.password = CodecHelpers::getOrElse(json, "password", "");
			obj.ip = CodecHelpers::getIp(json, "ip", IpAddress(192,168,1,1));
			obj.authMode = (WifiAuthMode) CodecHelpers::getOrElse<int>(json, "authMode", AUTH_OPEN);
			obj.hidden = CodecHelpers::getOrElse(json, "hidden", false);
			obj.channel = CodecHelpers::getOrElse(json, "channel", 7);
			obj.beaconInterval = CodecHelpers::getOrElse<unsigned int>(json, "beaconInterval", 200);
		}
		return obj;
	}
};