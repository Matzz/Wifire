#pragma once
#include <ArduinoJson.h>
#include "../Utils/Either.h"
#include "../Utils/NonCopyable.h"

#define JSON_MAX_SIZE 2048

template<typename T>
class Codec: private NonCopyable {
    public:
	static Codec<T>& getInstance() {
		static Codec<T> instance;
		return instance;
	}

    void encode(JsonObject& json, const T &obj);
    Either<String, T> decode(JsonObject& json);
    
    private:
        Codec() {}
        ~Codec() {}
};

class CodecHelpers {
    public:
    
    template<typename T>
    static void encodeDoc(JsonDocument& doc, const T& obj) {
		JsonObject json = doc.to<JsonObject>();
		Codec<T>::getInstance().encode(json, obj);
	}
	
    template<typename T>
    static Either<String, T> decodeDoc(JsonDocument& doc) {
		JsonObject json = doc.as<JsonObject>();
		return Codec<T>::getInstance().decode(json);
	}
    
    template<typename T>
	static T getOrElse(JsonObject& doc, const String& field, T elseVal) {
		return doc.containsKey(field) ? doc[field].as<T>() : elseVal;
	}

	static String ipToString(const IpAddress& ip) {
		return ip.isNull() ? String::nullstr : ip.toString();
	}

	static IpAddress getIp(JsonObject& json, String field, IpAddress defaultIp) {
		if(json.containsKey(field)) {
			String ipStr = json[field];
			return IpAddress(ipStr);
		} else {
			return defaultIp;
		}
	}
};