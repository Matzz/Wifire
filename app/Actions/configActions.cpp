#include <JsonObjectStream.h>
#include "configActions.h"
#include "../Services/Injector.h"
#include "../Services/Networking/AvaiableNetworksProvider.h"
#include "../Configuration/GPIOConfig.h"
#include "../Utils/utils.h"

void apGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiApConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	auto config = *configOrError.get_if_right();

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

	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}

void apSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiApConfigProvider();
	WiFiApConfig config;

	config.enabled = getBool(request, "enabled");
	config.ssid = request.getPostParameter("ssid");
	config.password = request.getPostParameter("password");
	config.authMode = (WifiAuthMode) request.getPostParameter("authMode").toInt();
	config.ip = IpAddress(request.getPostParameter("ip"));
	config.hidden = request.getPostParameter("hidden");
	config.channel = request.getPostParameter("channel").toInt();
	config.beaconInterval = request.getPostParameter("beaconInterval").toInt();
	provider.save(config);
	debug_i("AP config saved.");
}

void stationGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiStationConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	auto config = *configOrError.get_if_right();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	json["enabled"] = config.enabled;
	json["ssid"] = config.ssid;
	json["password"] = config.password;
	json["ip"] = config.ip.toString();
	json["netmask"] = config.netmask.toString();
	json["gateway"] = config.gateway.toString();

	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}

void stationSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiStationConfigProvider();
	WiFiStationConfig config;

	config.enabled = getBool(request, "enabled");
	config.ssid = request.getPostParameter("ssid");
	config.password = request.getPostParameter("password");
	config.ip = IpAddress(request.getPostParameter("ip"));
	config.netmask = IpAddress(request.getPostParameter("netmask"));
	config.gateway = IpAddress(request.getPostParameter("gateway"));
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
	response.setContentType(MIME_JSON);
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
	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}

void stationListNetworks(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiStationConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	auto config = *configOrError.get_if_right();
	
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	json["enabled"] = config.enabled;
	json["ssid"] = config.ssid;
	json["password"] = config.password;
	json["ip"] = config.ip.toString();
	json["netmask"] = config.netmask.toString();
	json["gateway"] = config.gateway.toString();

	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}

void otaGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getOtaConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	auto config = *configOrError.get_if_right();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	json["romUrl"] = config.romUrl;
	json["spiffUrl"] = config.spiffUrl;

	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}

void otaSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getOtaConfigProvider();
	OtaConfig config;
	config.romUrl = request.getPostParameter("romUrl");
	config.spiffUrl = request.getPostParameter("spiffUrl");
	provider.save(config);
	debug_i("OTA config saved.");
}

void gpioGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getGPIOConfigProvider();

	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	GPIOConfig config = *configOrError.get_if_right();

	JsonObjectStream* stream = new JsonObjectStream(JSON_MAX_SIZE);
	JsonObject json = stream->getRoot();
	Codec<GPIOConfig>::getInstance().encode(json, config);

	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}

String gpioFieldName(int idx, String field) {
	return "gpio[" + String(idx) + "]['"+ field +"']";
}

void gpioSetConfigAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	auto& provider = di.getGPIOConfigProvider();

	GPIOConfig config;
	for(int i=0; i<=GPIOConfig::max_pin; i++) {
		config.gpio[i].name = request.getPostParameter(gpioFieldName(i, "name"));
		config.gpio[i].isInput = getBool(request, gpioFieldName(i, "isInput"));
		config.gpio[i].pull = getBool(request, gpioFieldName(i, "pull"));
	}

	provider.save(config);
	debug_i("GPIO config saved.");
	di.getGPIOStateManager().update();
}
