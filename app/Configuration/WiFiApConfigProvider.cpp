#include "WiFiApConfigProvider.h"

#include <SmingCore.h>
#include "FileConfig.h"

WiFiApConfigProvider::WiFiApConfigProvider(String fileName) :
		FileConfig(fileName) {
}

WiFiApConfig WiFiApConfigProvider::load() {
	debug_i("Loading AP config.");
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	loadJsonObject(doc);
	WiFiApConfig cfg;
	String defaultSsid = "Wifire_" + String(system_get_chip_id(), 10);
	cfg.enabled = getOrElse(doc, "enabled", true);
	cfg.ssid = getOrElse(doc, "ssid", defaultSsid);
	cfg.password = getOrElse(doc, "password", "");
	cfg.ip = getIp(doc, "ip", IPAddress(192,168,13,10));
	cfg.authMode = (WifiAuthMode) getOrElse<int>(doc, "authMode", AUTH_OPEN);
	cfg.hidden = getOrElse(doc, "hidden", false);
	cfg.channel = getOrElse(doc, "channel", 7);
	cfg.beaconInterval = getOrElse(doc, "beaconInterval", 200);
	return cfg;
}

void WiFiApConfigProvider::save(WiFiApConfig cfg) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	doc["enabled"] = cfg.enabled;
	doc["ssid"] = cfg.ssid;
	doc["password"] = cfg.password;
	doc["authMode"] = (int) cfg.authMode;
	doc["ip"] = cfg.ip.toString();
	doc["hidden"] = cfg.hidden;
	doc["channel"] = cfg.channel;
	doc["beaconInterval"] = cfg.beaconInterval;
	saveJsonObject(doc);
}
