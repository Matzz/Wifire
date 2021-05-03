#pragma once
#include <SmingCore.h>
#include "../Utils/Either.h"
#include "../Model/Codec.h"
#include "../Services/ConfigProvider.h"

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

	String body = request.getBody();
	DynamicJsonDocument doc(JSON_MAX_SIZE);
	auto desRes = deserializeJson(doc, body);
	if(desRes != DeserializationError::Code::Ok) {
		return String(desRes.c_str());
	}
	auto objOrError = CodecHelpers::decodeDoc<T>(doc);
	return objOrError;
}

template<typename T>
void handleConfigGet(
	HttpRequest &request,
	HttpResponse &response,
	ConfigProvider<T> &cfgProvider,
	std::function<void(JsonDocument&)> jsonMapper = nullptr) {
	auto configOrError = cfgProvider.load();
	if(configOrError.isLeft()) {
		return returnFailure(response, *configOrError.getIfLeft());
	}
	auto config = *configOrError.getIfRight();
	handleConfigGet(request, response, config, jsonMapper);
}

template<typename T>
void handleConfigGet(
	HttpRequest &request,
	HttpResponse &response,
	const T &config,
	std::function<void(JsonDocument&)> jsonMapper = nullptr) {
	DynamicJsonDocument doc(JSON_MAX_SIZE);
	CodecHelpers::encodeDoc<T>(doc, config);
	if(jsonMapper != nullptr) {
		jsonMapper(doc);
	}
	String body;
	serializeJson(doc, body);

	response.setContentType(MIME_JSON);
	response.sendString(body);
}

template<typename T>
void handleConfigSet(HttpRequest &request, HttpResponse &response, ConfigProvider<T> &cfgProvider) {
	Either<String, T> configOrError = decodeJson<T>(request);
	if(configOrError.isLeft()) {
		return returnFailure(response, *configOrError.getIfLeft());
	}
	T config = *configOrError.getIfRight();
	
	cfgProvider.save(config);
	debug_i("Config saved.");
	returnOk(response, "Config saved.");
}