#include "configActions.h"
#include <SmingCore.h>
#include <ArduinoJson.h>
#include <JsonObjectStream.h>
#include "../Services/Injector.h"
#include "../Services/Networking/AvaiableNetworksProvider.h"
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
	auto updater = di.getOtaUpdater();
	updater.update();

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
		response.code = HttpStatus::ACCEPTED;
		json["message"] = F("Scanning in progress...");
	} else {
		response.code = HttpStatus::ACCEPTED;
		json["message"] = F("Scanning started.");
	}
	AvaiableNetworksProvider::startScan();
	response.setContentType(MIME_JSON);
	response.sendNamedStream(stream);
}

void stationGetNetworks(HttpRequest &request, HttpResponse &response) {
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	if(!AvaiableNetworksProvider::isScanning()) {
		if(!AvaiableNetworksProvider::wasEverScanned) {
			AvaiableNetworksProvider::startScan();
			response.code = HttpStatus::ACCEPTED;
			json["message"] = F("Scanning in progress...");
		}
		else if(AvaiableNetworksProvider::networks.isEmpty()) {
			AvaiableNetworksProvider::startScan();
			response.code = HttpStatus::NO_CONTENT;
			json["message"] = F("No networks. Scanning again.");
		} else {
			response.code = HttpStatus::OK;
			json["scanned_seconds_ago"] = AvaiableNetworksProvider::lastScanAgeSeconds();
			JsonArray networksJson = json.createNestedArray("networks");
			BssList networks = AvaiableNetworksProvider::networks;
			for(int i=0; i<networks.size(); i++) {
				BssInfo network = networks[i];
				JsonObject networkJson = networksJson.createNestedObject();
				networkJson["id"] = network.getHashId();
				networkJson["ssid"] = network.ssid;
				networkJson["bssid"] = network.bssid;
				networkJson["authorization"] = network.getAuthorizationMethodName();
				networkJson["channel"] = network.channel;
				networkJson["channel"] = network.channel;
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