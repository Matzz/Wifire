#include <JsonObjectStream.h>
#include "configActions.h"
#include "../Services/Injector.h"
#include "../Services/Networking/AvaiableNetworksProvider.h"
#include "../Utils/utils.h"

void apGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiApConfigProvider();
	auto config = provider.load();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	json["enabled"] = config.enabled;
	json["ssid"] = config.ssid;
	json["password"] = config.password;
	json["authMode"] = (uint8) config.authMode;
	json["ip"] = config.ip.toString();
	json["hidden"] = config.hidden;
	json["channel"] = config.channel;
	json["beaconInterval"] = config.beaconInterval;

	response.sendNamedStream(stream);
}

void apSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiApConfigProvider();
	auto config = provider.load();

	config.enabled = getBool(request, "enabled");
	config.ssid = request.getPostParameter("ssid");
	config.password = request.getPostParameter("password");
	config.authMode = (WifiAuthMode) request.getPostParameter("authMode").toInt();
	config.ip = IPAddress(request.getPostParameter("ip"));
	config.hidden = request.getPostParameter("hidden");
	config.channel = request.getPostParameter("channel").toInt();
	config.beaconInterval = request.getPostParameter("beaconInterval").toInt();
	provider.save(config);
	debug_i("AP config saved.");
}

void stationGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiStationConfigProvider();
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
	auto& provider = Injector::getInstance().getWiFiStationConfigProvider();
	auto config = provider.load();

	config.enabled = getBool(request, "enabled");
	config.ssid = request.getPostParameter("ssid");
	config.password = request.getPostParameter("password");
	config.ip = IPAddress(request.getPostParameter("ip"));
	config.netmask = IPAddress(request.getPostParameter("netmask"));
	config.gateway = IPAddress(request.getPostParameter("gateway"));
	provider.save(config);
	debug_i("Station config saved.");
}

void stationRefreshNetworks(HttpRequest &request, HttpResponse &response) {
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	if(AvaiableNetworksProvider::isScanning()) {
			response.code = HttpStatus::ALREADY_REPORTED;
			json["message"] = "No networks. Scanning again.";
	} else {
			json["message"] = "Scanning started.";
	}
	AvaiableNetworksProvider::startScan();
	response.sendNamedStream(stream);
}

void stationGetNetworks(HttpRequest &request, HttpResponse &response) {
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	if(!AvaiableNetworksProvider::isScanning()) {
		if(AvaiableNetworksProvider::networks.isEmpty()) {
			AvaiableNetworksProvider::startScan();
			response.code = HttpStatus::NO_CONTENT;
			json["message"] = "No networks. Scanning again.";
		} else {
			JsonArray networksJson = json.createNestedArray("networks");
			BssList networks = AvaiableNetworksProvider::networks;
			for(int i=0; i<networks.size(); i++) {
				BssInfo network = networks[i];
				JsonObject networkJson = networksJson.createNestedObject();
				networkJson["ssid"] = network.ssid;
				networkJson["bssid"] = network.bssid;
				networkJson["authorization"] = network.authorization;
				networkJson["channel"] = network.channel;
				networkJson["channel"] = network.channel;
				networkJson["rssi"] = network.rssi;
				networkJson["hidden"] = network.hidden;
			}
		}
	} else {
		json["message"] = "Scanning in progress.";
	}
	response.sendNamedStream(stream);
}

void stationListNetworks(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiStationConfigProvider();
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

void otaGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getOtaConfigProvider();
	auto config = provider.load();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	json["romUrl"] = config.romUrl;
	json["spiffUrl"] = config.spiffUrl;

	response.sendNamedStream(stream);
}

void otaSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getOtaConfigProvider();
	auto config = provider.load();

	config.romUrl = request.getPostParameter("romUrl");
	config.spiffUrl = request.getPostParameter("spiffUrl");
	provider.save(config);
	debug_i("OTA config saved.");
}

void gpioGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getGPIOConfigProvider();
	auto config = provider.load();

	JsonObjectStream* stream = new JsonObjectStream(JSON_MAX_SIZE);
	JsonObject json = stream->getRoot();
	GPIOConfigProvider::configToJson(config, json);
	response.sendNamedStream(stream);
}

String gpioFieldName(int idx, String field) {
	return "gpio[" + String(idx) + "]['"+ field +"']";
}

void gpioSetConfigAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	auto& provider = di.getGPIOConfigProvider();

	auto config = provider.load();
	for(int i=0; i<=PIN_MAX; i++) {
		config.gpio[i].name = request.getPostParameter(gpioFieldName(i, "name"));
		config.gpio[i].isInput = getBool(request, gpioFieldName(i, "isInput"));
		config.gpio[i].pull = getBool(request, gpioFieldName(i, "pull"));
	}

	provider.save(config);
	debug_i("GPIO config saved.");
	di.getGPIOStateManager().update();
}
