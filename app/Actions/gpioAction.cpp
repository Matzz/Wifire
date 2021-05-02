#include "gpioAction.h"
#include "../Services/GPIOStateManager.h"
#include "../Services/Injector.h"
#include "actionsHelpers.h"

void currentState(ConfigProvider<GPIOConfig> &cfgProvider, JsonDocument& doc) {
	auto configOrError = cfgProvider.load();
	if(configOrError.isLeft()) {
		debugf("currentState get config returned an error - %s", configOrError.getIfLeft());
		return;
	}
	auto config = *configOrError.getIfRight();

	auto obj = doc.as<JsonObject>();
	auto state = obj.createNestedObject("state");
	for(int i=0; i<=GPIOConfig::max_pin; i++) {
		PinConfig& pinCfg = config.gpio[i];
		if(!pinCfg.isInput) { // and credentials
			state[String(i)] = digitalRead(i);
		} else {
			state[String(i)] = 0;
		}
	}
	
}

void addSafePinsToJson(JsonDocument& doc) {
		auto obj = doc.as<JsonObject>();
		auto gpio = obj["gpio"];
		for(int i=0; i<=GPIOConfig::max_pin; i++) {
			bool isSafe = GPIOConfig::isSafeToUse(i);
			gpio[String(i)]["isSafe"] = isSafe;
		}

}

void gpioGetConfigAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getGPIOConfigProvider();
	handleConfigGet(request, response, provider, addSafePinsToJson);
}

void gpioSetConfigAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	auto& provider = di.getGPIOConfigProvider();
	handleConfigSet(request, response, provider);
	di.getGPIOStateManager().update();
}


void gpioSwitchAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	auto &provider = di.getGPIOConfigProvider();
	GPIOStateManager& gpioState = di.getGPIOStateManager();
	String name = request.getQueryParameter("name");
	uint32_t howLongMs = request.getQueryParameter("time_ms", "0").toInt();
	
	String stateName = request.getQueryParameter("state", "on");
	stateName.toLowerCase();
	uint8_t state = stateName == "on" ? HIGH : LOW;
	
    bool result = gpioState.switchPin(name, state, howLongMs);

	DynamicJsonDocument doc(JSON_MAX_SIZE);
	doc.as<JsonObject>();
	doc["result"] = result;
	currentState(provider, doc);
	String body;
	serializeJson(doc, body);

	response.setContentType(MIME_JSON);
	response.sendString(body);

}

void gpioSwitchGetAvailableAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	auto &provider = di.getGPIOConfigProvider();
	auto jsonMapper = [](JsonDocument& doc) {
		auto obj = doc.as<JsonObject>();
		JsonArray gpios = obj["gpio"];
		for(auto pin : gpios) {
			// Filter only those pins which are accessible according to user role
		}

		Injector &di = Injector::getInstance();
		auto &provider = di.getGPIOConfigProvider();	
		addSafePinsToJson(doc);
		currentState(provider, doc);
	};
	handleConfigGet(request, response, provider, jsonMapper);
}