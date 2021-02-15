#pragma once

#include <SmingCore.h>
#include <ArduinoJson.h>

class StringVectorCodec {
	public:
	static void encode(JsonObject& json, const Vector<String>& vector, const String& key);

	static Vector<String> decode(JsonObject& json, const String& key);
};