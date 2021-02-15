#pragma once
#include <ArduinoJson.h>
#include "../Utils/Either.h"
#include "../Utils/NonCopyable.h"

#define JSON_MAX_SIZE 2048

template<typename T>
class Codec: private NonCopyable {
    public:
    static T& getInstance();

    virtual void encode(JsonObject& json, T obj) = 0;
    virtual Either<String, T> decode(JsonObject& json) = 0;
    
    private:
        Codec() {}
        ~Codec() {}
};

class CodecHelpers {
    public:
    
    template<typename T>
    static void encodeDoc(Codec<T>& codec, JsonDocument& doc, T& obj) {
		JsonObject json = doc.to<JsonObject>();
		codec.encode(json, obj);
	}
	
    template<typename T>
    static Either<String, T> decodeDoc(Codec<T>& codec, JsonDocument& doc) {
		JsonObject json = doc.as<JsonObject>();
		return codec.decode(json);
	}
    
    template<typename T>
	static T getOrElse(JsonObject& doc, const String& field, T elseVal) {
		return doc.containsKey(field) ? doc[field].as<T>() : elseVal;
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