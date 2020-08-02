#include "FileConfig.h"
#include "c_types.h"
#include <JsonObjectStream.h>

FileConfig::FileConfig(String fileName) : fileName(fileName) {
}

void FileConfig::saveJsonObject(JsonDocument& doc) {
	String output;
    serializeJson(doc, output);
	fileSetContent(fileName, output);
}

void FileConfig::loadJsonObject(JsonDocument& doc) {
	String jsonString;
	if (fileExist(fileName)) {
		jsonString = fileGetContent(fileName);
	} else {
		debug_i("File %s doesn't exist.", fileName.c_str());
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
