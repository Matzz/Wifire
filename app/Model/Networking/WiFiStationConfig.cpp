#include "WiFiStationConfig.h"
#include <SmingCore.h>

template<>
void Codec<WiFiStationConfig>::encode(JsonObject& json, const WiFiStationConfig &obj) {
	json["enabled"] = obj.enabled;
	json["ssid"] = obj.ssid;
	json["password"] = obj.password;
	json["hostname"] = obj.hostname;
	json["ip"] = CodecHelpers::ipToString(obj.ip);
	json["netmask"] = CodecHelpers::ipToString(obj.netmask);
	json["gateway"] = CodecHelpers::ipToString(obj.gateway);
}
template<>
Either<String, WiFiStationConfig> Codec<WiFiStationConfig>::decode(JsonObject& json) {
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