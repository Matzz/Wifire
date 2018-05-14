#include "WiFiStationConfigProvider.h"

#include <SmingCore/SmingCore.h>
#include "FileConfig.h"


WiFiStationConfigProvider::WiFiStationConfigProvider(String fileName) :
		FileConfig(fileName) {
}

WiFiStationConfig WiFiStationConfigProvider::load() {
	Serial.println("Loading station config.");
	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject& json = loadJsonObject(jsonBuffer);
	WiFiStationConfig cfg;
	cfg.enabled = getOrElse(json, "enabled", true);
	cfg.ssid = getOrElse(json, "ssid", "");
	cfg.password = getOrElse(json, "password", "");
	cfg.ip = getIp(json, "ip", IPAddress(192, 168, 1, 1));
	cfg.netmask = getIp(json, "netmask", IPAddress(255, 255, 255, 0));
	cfg.gateway = getIp(json, "gateway", IPAddress(192, 168, 1, 1));
	return cfg;
}

void WiFiStationConfigProvider::save(WiFiStationConfig cfg) {
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["enabled"] = cfg.enabled;
	root["ssid"] = cfg.ssid;
	root["password"] = cfg.password;
	root["ip"] = cfg.ip.toString();
	root["netmask"] = cfg.netmask.toString();
	root["gateway"] = cfg.gateway.toString();
	saveJsonObject(root);
}
