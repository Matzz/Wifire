#include "WiFiApConfig.h"
#include <SmingCore.h>

template<>
void Codec<WiFiApConfig>::encode(JsonObject& json, const WiFiApConfig &obj) {
	json["enabled"] = obj.enabled;
	json["ssid"] = obj.ssid;
	json["password"] = obj.password;
	json["authMode"] = (int) obj.authMode;
	json["ip"] = CodecHelpers::ipToString(obj.ip);
	json["hidden"] = obj.hidden;
	json["channel"] = obj.channel;
	json["beaconInterval"] = obj.beaconInterval;
}

template<>
Either<String, WiFiApConfig> Codec<WiFiApConfig>::decode(JsonObject& json) {
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