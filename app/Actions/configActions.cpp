#include <JsonObjectStream.h>
#include "configActions.h"
#include "../Services/Injector.h"


void apGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto provider = Injector::getInstance().getWiFiApConfigProvider();
	auto config = provider.load();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	json["enabled"] = config.enabled;
	json["ssid"] = config.ssid;
	json["password"] = config.password;
	json["authMode"] = config.authMode +'0';
	json["ip"] = config.ip.toString();
	json["hidden"] = config.hidden;
	json["channel"] = config.channel;
	json["beaconInterval"] = config.beaconInterval;

	response.sendNamedStream(stream);
}

void apSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto provider = Injector::getInstance().getWiFiApConfigProvider();
	auto config = provider.load();

	config.enabled = request.getPostParameter("enabled");
	config.ssid = request.getPostParameter("ssid");
	config.password = request.getPostParameter("password");
	config.authMode = (WifiAuthMode) request.getPostParameter("authMode").toInt();
	config.ip = IPAddress(request.getPostParameter("ip"));
	config.hidden = request.getPostParameter("hidden");
	config.channel = request.getPostParameter("channel").toInt();
	config.beaconInterval = request.getPostParameter("beaconInterval").toInt();
	provider.save(config);
	Serial.println("AP config saved.");

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	json["status"] = "successful";
	response.sendNamedStream(stream);
}

void stationGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto provider = Injector::getInstance().getWiFiStationConfigProvider();
	auto config = provider.load();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	json["enabled"] = config.enabled;
	json["ssid"] = config.ssid;
	json["password"] = config.password;
	json["ip"] = config.ip.toString();
	json["netmask"] = config.netmask.toString();
	json["gateway"] = config.gateway.toString();

	response.sendNamedStream(stream);
}

void stationSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto provider = Injector::getInstance().getWiFiStationConfigProvider();
	auto config = provider.load();

	config.enabled = request.getPostParameter("enabled");
	config.ssid = request.getPostParameter("ssid");
	config.password = request.getPostParameter("password");
	config.ip = IPAddress(request.getPostParameter("ip"));
	config.netmask = IPAddress(request.getPostParameter("netmask"));
	config.gateway = IPAddress(request.getPostParameter("gateway"));
	provider.save(config);
	Serial.println("Station config saved.");

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	json["status"] = "successful";
	response.sendNamedStream(stream);
}

void otaGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto provider = Injector::getInstance().getOtaConfigProvider();
	auto config = provider.load();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	json["romUrl"] = config.romUrl;
	json["spiffUrl"] = config.spiffUrl;

	response.sendNamedStream(stream);
}

void otaSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto provider = Injector::getInstance().getOtaConfigProvider();
	auto config = provider.load();

	config.romUrl = request.getPostParameter("romUrl");
	config.spiffUrl = request.getPostParameter("spiffUrl");
	provider.save(config);
	Serial.println("OTA config saved.");

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	json["status"] = "successful";
	response.sendNamedStream(stream);
}
