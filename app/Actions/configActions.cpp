#include "configActions.h"
#include <ArduinoJson.h>
#include <JsonObjectStream.h>
#include "../Services/Injector.h"
#include "../Services/Networking/AvaiableNetworksProvider.h"
#include "../Configuration/GPIOConfig.h"
#include "../Configuration/OtaConfig.h"
#include "../Configuration/Networking/WiFiApConfig.h"
#include "../Configuration/Networking/WiFiStationConfig.h"
#include "../Configuration/Users/UsersConfig.h"

void apGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiApConfigProvider();
	handleConfigGet(request, response, provider);
}

void apSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiApConfigProvider();
	handleConfigSet(request, response, provider);
}

void stationGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiStationConfigProvider();
	handleConfigGet(request, response, provider);
}

void stationSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getWiFiStationConfigProvider();
	handleConfigSet(request, response, provider);
}

void otaGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getOtaConfigProvider();
	handleConfigGet(request, response, provider);
}

void otaSetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getOtaConfigProvider();
	handleConfigSet(request, response, provider);
}

void gpioGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getGPIOConfigProvider();
	handleConfigGet(request, response, provider);
}

void gpioSetConfigAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	auto& provider = di.getGPIOConfigProvider();
	handleConfigSet(request, response, provider);
	di.getGPIOStateManager().update();
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