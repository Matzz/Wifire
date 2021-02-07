#pragma once

#include <SmingCore.h>
#include "Codec.h"

class OtaConfig {
public:
	String romUrl;
	String spiffUrl;
};

template<> class Codec<OtaConfig> {
	public:
        static Codec<OtaConfig>& getInstance() {
            static Codec<OtaConfig> instance;
            return instance;
        }

	void encode(JsonObject& json, OtaConfig obj) {
		json["romUrl"] = obj.romUrl.c_str();
		json["spiffUrl"] = obj.spiffUrl.c_str();
	}

	OtaConfig decode(JsonObject& json) {
		OtaConfig cfg;
		cfg.romUrl = CodecHelpers::getOrElse(json, "romUrl", String::empty);
		cfg.spiffUrl = CodecHelpers::getOrElse(json, "spiffUrl", String::empty);
		return cfg;
	}
};