#include "WiFiStationConfigProvider.h"

#include <SmingCore.h>


WiFiStationConfigProvider::WiFiStationConfigProvider(String fileName) :
		FileConfig(fileName) {
}

WiFiStationConfig WiFiStationConfigProvider::load() {
	debug_i("Loading station config.");
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	loadJsonObject(doc);
	WiFiStationConfig cfg;
	cfg.enabled = getOrElse(doc, "enabled", false);
	cfg.ssid = getOrElse(doc, "ssid", "");
	cfg.password = getOrElse(doc, "password", "");
	cfg.ip = getIp(doc, "ip", IPAddress(192, 168, 1, 1));
	cfg.netmask = getIp(doc, "netmask", IPAddress(255, 255, 255, 0));
	cfg.gateway = getIp(doc, "gateway", IPAddress(192, 168, 1, 1));
	return cfg;
}

void WiFiStationConfigProvider::save(WiFiStationConfig cfg) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	doc["enabled"] = cfg.enabled;
	doc["ssid"] = cfg.ssid;
	doc["password"] = cfg.password;
	doc["ip"] = cfg.ip.toString();
	doc["netmask"] = cfg.netmask.toString();
	doc["gateway"] = cfg.gateway.toString();
	saveJsonObject(doc);
}
