#include "FileConfig.h"

#include <SmingCore/SmingCore.h>

FileConfig::FileConfig(String fileName) : fileName(fileName) {
}

void FileConfig::saveJsonObject(JsonObject& json) {
	auto length = json.measureLength()+10;
	char *buffer = new char[length];
	json.printTo(buffer, length);
	fileSetContent(fileName, buffer);
	delete[] buffer;
}

JsonObject& FileConfig::loadJsonObject(JsonBuffer& jsonBuffer) {
	String jsonString;
	if (fileExist(fileName)) {
		jsonString = fileGetContent(fileName);

	} else {
		jsonString = String("{}");
	}
	JsonObject& jsonObj = jsonBuffer.parseObject(jsonString);
	return jsonObj;
}

IPAddress FileConfig::getIp(JsonObject& json, String field, IPAddress defaultIp) {
	if(json.containsKey(field)) {
		String ipStr = json[field].asString();
		return IPAddress(ipStr);
	} else {
		return defaultIp;
	}
}
