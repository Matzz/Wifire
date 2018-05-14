#ifndef APP_SERVICES_FILECONFIG_H_
#define APP_SERVICES_FILECONFIG_H_
#include <SmingCore/SmingCore.h>

class FileConfig {
protected:
	String fileName;

	FileConfig(String fileName);

	void saveJsonObject(JsonObject& json);

	JsonObject& loadJsonObject(JsonBuffer& jsonBuffer);

	template<typename T>
	T getOrElse(ArduinoJson::JsonObject& json, String field, T elseVal) {
		return json.containsKey(field) ? json[field].as<T>() : elseVal;
	}

	IPAddress getIp(JsonObject& json, String field, IPAddress defaultIp = IPAddress());
};

#endif
