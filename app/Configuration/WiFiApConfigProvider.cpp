#include "WiFiApConfigProvider.h"

#include <SmingCore/SmingCore.h>
#include "FileConfig.h"

WiFiApConfigProvider::WiFiApConfigProvider(String fileName) :
		FileConfig(fileName) {
}

WiFiApConfig WiFiApConfigProvider::load() {
	Serial.println("Loading AP config.");
	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject& json = loadJsonObject(jsonBuffer);
	WiFiApConfig cfg;
	String defaultSsid = "Wifire_" + String(system_get_chip_id(), 10);
	cfg.enabled = getOrElse(json, "enabled", true);
	cfg.ssid = getOrElse(json, "ssid", defaultSsid);
	cfg.password = getOrElse(json, "password", "");
	cfg.ip = getIp(json, "ip", IPAddress(192, 168, 1, 1));
	cfg.authMode = (_auth_mode) getOrElse<int>(json, "authMode", AUTH_OPEN);
	cfg.hidden = getOrElse(json, "hidden", false);
	cfg.channel = getOrElse(json, "channel", 7);
	cfg.beaconInterval = getOrElse(json, "beaconInterval", 200);
	return cfg;
}

void WiFiApConfigProvider::save(WiFiApConfig cfg) {
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["enabled"] = cfg.enabled;
	root["ssid"] = cfg.ssid;
	root["password"] = cfg.password;
	root["authMode"] = (int) cfg.authMode;
	root["ip"] = cfg.ip.toString();
	root["hidden"] = cfg.hidden;
	root["channel"] = cfg.channel;
	root["beaconInterval"] = cfg.beaconInterval;
	saveJsonObject(root);
}
