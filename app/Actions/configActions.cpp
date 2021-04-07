#include "configActions.h"
#include <SmingCore.h>
#include <ArduinoJson.h>
#include <JsonObjectStream.h>
#include "../Services/Injector.h"
#include "../Model/GPIOConfig.h"
#include "../Model/OtaConfig.h"
#include "../Model/Networking/WiFiApConfig.h"
#include "../Model/Networking/WiFiStationConfig.h"
#include "../Model/Users/UsersConfig.h"

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
	Injector &di = Injector::getInstance();
	auto& provider = di.getOtaConfigProvider();
	handleConfigGet(request, response, provider);

}

void otaSetConfigAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	auto& provider = di.getOtaConfigProvider();
	handleConfigSet(request, response, provider);
	// auto& updater = di.getOtaUpdater();
	// updater.update();
}

void gpioGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getGPIOConfigProvider();
	auto jsonMapper = [](JsonDocument& doc) {
		auto obj = doc.as<JsonObject>();
		JsonArray safePins = obj.createNestedArray("safePins");
		for(const auto pin: GPIOConfig::safe_pins) {
			safePins.add(pin);
		}
	};
	handleConfigGet(request, response, provider, jsonMapper);
}

void gpioSetConfigAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	auto& provider = di.getGPIOConfigProvider();
	handleConfigSet(request, response, provider);
	di.getGPIOStateManager().update();
}

void stationRefreshNetworks(HttpRequest &request, HttpResponse &response) {
	AvaiableNetworksProvider& networksProvider = Injector::getInstance().getAvaiableNetworksProvider();
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	if(networksProvider.isScanning()) {
		response.code = HttpStatus::ACCEPTED;
		json["message"] = F("Scanning in progress...");
	} else {
		response.code = HttpStatus::ACCEPTED;
		json["message"] = F("Scanning started.");
	}
	networksProvider.startScan();
	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}

void stationGetNetworks(HttpRequest &request, HttpResponse &response) {
	AvaiableNetworksProvider& networksProvider = Injector::getInstance().getAvaiableNetworksProvider();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	if(!networksProvider.isScanning()) {
		BssList networks = networksProvider.getNetworks();
		if(!networksProvider.wasEverScanned()) {
			networksProvider.startScan();
			response.code = HttpStatus::ACCEPTED;
			json["message"] = F("Scanning in progress...");
		}
		else if(networks.isEmpty()) {
			networksProvider.startScan();
			response.code = HttpStatus::NO_CONTENT;
			json["message"] = F("No networks. Scanning again.");
		} else {
			response.code = HttpStatus::OK;
			json["scanned_seconds_ago"] = networksProvider.lastScanAgeSeconds();
			JsonArray networksJson = json.createNestedArray("networks");
			for(int i=0; i<networks.size(); i++) {
				BssInfo network = networks[i];
				JsonObject networkJson = networksJson.createNestedObject();
				networkJson["ssid"] = network.ssid;
				networkJson["authorization"] = network.getAuthorizationMethodName();
				networkJson["rssi"] = network.rssi;
				networkJson["hidden"] = network.hidden;
			}
		}
	} else {
		response.code = HttpStatus::ACCEPTED;
		json["message"] = F("Scanning in progress...");
	}
	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}