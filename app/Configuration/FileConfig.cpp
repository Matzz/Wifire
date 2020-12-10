#include "FileConfig.h"
#include "c_types.h"
#include <JsonObjectStream.h>

FileConfig::FileConfig(String fileName) : fileName(fileName) { }

void FileConfig::saveJsonObject(JsonDocument& json) {
	String output;
    serializeJson(json, output);
	fileSetContent(fileName, output);
}

void FileConfig::loadJsonObject(JsonDocument& json) {
	String jsonString;
	if (fileExist(this->fileName)) {
		jsonString = fileGetContent(this->fileName);
	} else {
		debug_i("File '%s' doesn't exist.", this->fileName.c_str());
		jsonString = String("{}");
	}
	deserializeJson(json, jsonString);
}

IpAddress FileConfig::getIp(JsonDocument& json, String field, IpAddress defaultIp) {
	if(json.containsKey(field)) {
		String ipStr = json[field];
		return IpAddress(ipStr);
	} else {
		return defaultIp;
	}
}
