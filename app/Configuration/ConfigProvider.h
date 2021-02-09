#pragma once

#include <SmingCore.h>
#include <JsonObjectStream.h>
#include <ArduinoJson.h>
#include "../Utils/NonCopyable.h"
#include "../Utils/Either.h"
#include "Codec.h"

template <typename T>
class ConfigProvider: private NonCopyable {
protected:
	String fileName = "none";
	Codec<T>& codec;
public:
	ConfigProvider(String fileName, Codec<T> &codec) : fileName(fileName), codec(codec) { }
	void save(T& obj) {
		DynamicJsonDocument doc(JSON_MAX_SIZE);
		String output;
		CodecHelpers::encodeDoc(codec, doc, obj);
		serializeJson(doc, output);
		fileSetContent(fileName, output);
	}
	
	Either<String, T> load() {
		DynamicJsonDocument doc(JSON_MAX_SIZE);
		debug_i("Loading config: %s", fileName.c_str());
		if (fileExist(fileName)) {
			if(!Json::loadFromFile(doc, fileName)) {
				return F("Invalid json.");
			}
		} else {
			// We still try to decode empty result. Maybe there are sensible defaults in the decoder.
			debug_i("File '%s' doesn't exist.", this->fileName.c_str());
		}
		
		return CodecHelpers::decodeDoc(codec, doc);
	}
};