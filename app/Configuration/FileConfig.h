#pragma once

#include <SmingCore.h>
#include <JsonObjectStream.h>
#include <ArduinoJson.h>
#define JSON_MAX_SIZE 2048

class FileConfig {
protected:
	String fileName;

	FileConfig(String fileName);

	void saveJsonObject(JsonDocument& json);

	void loadJsonObject(JsonDocument& doc);

	template<typename T>
	T getOrElse(JsonDocument& doc, String field, T elseVal) {
		return doc.containsKey(field) ? doc[field].as<T>() : elseVal;
	}

	IPAddress getIp(JsonDocument& json, String field, IPAddress defaultIp = IPAddress());
};
