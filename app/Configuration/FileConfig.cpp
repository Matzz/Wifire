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

IPAddress FileConfig::getIp(JsonDocument& json, String field, IPAddress defaultIp) {
	if(json.containsKey(field)) {
		String ipStr = json[field];
		return IPAddress(ipStr);
	} else {
		return defaultIp;
	}
}
