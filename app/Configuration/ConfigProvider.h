#pragma once

#include <SmingCore.h>
#include <JsonObjectStream.h>
#include <ArduinoJson.h>
#include "../Utils/NonCopyable.h"
#include "../Utils/Either.h"
#include "Codec.h"
#define JSON_MAX_SIZE 2048

template <typename T>
class ConfigProvider: private NonCopyable {
protected:
	String fileName = "none";
	Codec<T>& codec;
public:
	ConfigProvider(String fileName, Codec<T> &codec) : fileName(fileName), codec(codec) { }
	void save(T& obj) {
		StaticJsonDocument<JSON_MAX_SIZE> doc;
		String output;
		CodecHelpers::encodeDoc(codec, doc, obj);
		serializeJson(doc, output);
		fileSetContent(fileName, output);
	}
	
	Either<String, T> load() {
		String jsonString;
		debug_i("Loading config: %s", this->fileName.c_str());
		if (fileExist(this->fileName)) {
			jsonString = fileGetContent(this->fileName);
		} else {
			debug_i("File '%s' doesn't exist.", this->fileName.c_str());
			jsonString = String("{}");
		}
		StaticJsonDocument<JSON_MAX_SIZE> doc;
		deserializeJson(doc, jsonString);
		return CodecHelpers::decodeDoc(codec, doc);
	}
};