#pragma once

#include <SmingCore.h>
#include <JsonObjectStream.h>
#include <ArduinoJson.h>
#include "../Utils/NonCopyable.h"
#include "../Utils/Either.h"
#include "../Model/Codec.h"

template <typename T>
class ConfigProvider: private NonCopyable {
public:
	virtual void save(T& obj);
	virtual Either<String, T> load();
};


template <typename T>
class FileConfigProvider: public ConfigProvider<T> {
protected:
	String fileName;
public:
	FileConfigProvider(String fileName) : fileName(fileName) { }

	void save(T& obj) override {
		DynamicJsonDocument doc(JSON_MAX_SIZE);
		String output;
		CodecHelpers::encodeDoc<T>(doc, obj);
		serializeJson(doc, output);
		fileSetContent(fileName, output);
	}
	
	Either<String, T> load() override {
		DynamicJsonDocument doc(JSON_MAX_SIZE*2);
		debug_i("Loading config: %s", fileName.c_str());
		if (fileExist(fileName)) {
			if(!Json::loadFromFile(doc, fileName)) {
				return F("Invalid json.");
			}
		} else {
			// We still try to decode empty result. Maybe there are sensible defaults in the decoder.
			debug_i("File '%s' doesn't exist.", this->fileName.c_str());
		}
		
		return CodecHelpers::decodeDoc<T>(doc);
	}
};

/**
 * This class cache configuration loaded from file to reduce CPU overhead when we require the same config multiple times.
 * Be aware about the memory overhead of caching.
 **/
template <typename T>
class CachedConfigProvider: public ConfigProvider<T> {
protected:
	ConfigProvider<T> &innerProvider;
	bool isAvailable = false;
	T config;
public:
	CachedConfigProvider(ConfigProvider<T> &innerProvider) : innerProvider(innerProvider) { }

	void save(T& obj) override {
		config = obj;
		innerProvider.save(obj);
	}
	
	Either<String, T> load() override {
		if(!isAvailable) {
			Either<String, T> configOrError = innerProvider.load();
			if(configOrError.isLeft()) {
				isAvailable = false;
				return configOrError; // Do not cache errors
			} else {
				isAvailable = true;
				config = *configOrError.getIfRight();
			}
		}
		return {RightTagT(), config};
	}
};