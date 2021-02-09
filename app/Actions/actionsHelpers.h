#pragma once
#include <SmingCore.h>
#include "../Utils/Either.h"
#include "../Configuration/Codec.h"
#include "../Configuration/ConfigProvider.h"

bool getBool(HttpRequest& request, const String& name);
String getString(HttpRequest& request, const String &name, const String& defaultVal = "");

void returnOk(HttpResponse &response, String msg);
void returnFailure(HttpResponse &response, String msg);

const String getSessionId(HttpRequest& request);

template<typename T>
Either<String, T> decodeJson(HttpRequest &request) {
	if(request.getBodyStream() == nullptr) {
		return F("Received an empty json.");
	}
	DynamicJsonDocument doc(JSON_MAX_SIZE);
	String body = request.getBody();
	auto desRes = deserializeJson(doc, body);
	if(!Json::deserialize(doc, body)) {
		return F("Invalid json.");
	}
	auto objOrError = CodecHelpers::decodeDoc(Codec<T>::getInstance(), doc);
	return objOrError;
}

template<typename T>
void handleConfigGet(HttpRequest &request, HttpResponse &response, ConfigProvider<T> &cfgProvider) {
	auto configOrError = cfgProvider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, *configOrError.get_if_left());
	}
	auto config = *configOrError.get_if_right();

	DynamicJsonDocument doc(JSON_MAX_SIZE);
	CodecHelpers::encodeDoc<T>(Codec<T>::getInstance(), doc, config);
	String body;
	serializeJson(doc, body);

	response.setContentType(MIME_JSON);
	response.sendString(body);
}

template<typename T>
void handleConfigSet(HttpRequest &request, HttpResponse &response, ConfigProvider<T> &cfgProvider) {
	Either<String, T> configOrError = decodeJson<T>(request);
	if(configOrError.is_left()) {
		return returnFailure(response, *configOrError.get_if_left());
	}
	T config = *configOrError.get_if_right();
	
	cfgProvider.save(config);
	debug_i("Config saved.");
	returnOk(response, "Config saved.");
}