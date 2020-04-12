#include "FileConfig.h"
#include "c_types.h"
#include <JsonObjectStream.h>

FileConfig::FileConfig(String fileName) : fileName(fileName) {
}

void FileConfig::saveJsonObject(JsonDocument& doc) {
	auto length = measureJson(doc)+10;
	char *buffer = new char[length];
	serializeJson(doc, buffer, length);
	fileSetContent(fileName, buffer);
	delete[] buffer;
}

void FileConfig::loadJsonObject(JsonDocument& doc) {
	String jsonString;
	if (fileExist(fileName)) {
		jsonString = fileGetContent(fileName);

	} else {
		jsonString = String("{}");
	}
	deserializeJson(doc, jsonString);
}

IPAddress FileConfig::getIp(JsonDocument& doc, String field, IPAddress defaultIp) {
	if(doc.containsKey(field)) {
		String ipStr = doc[field];
		return IPAddress(ipStr);
	} else {
		return defaultIp;
	}
}
