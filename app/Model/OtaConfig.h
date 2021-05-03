#pragma once

#include <SmingCore.h>
#include "Codec.h"

class OtaConfig {
public:
	String romUrl;
	String spiffUrl;
};

template<>
void Codec<OtaConfig>::encode(JsonObject& json, const OtaConfig &cfg);

template<>
Either<String, OtaConfig> Codec<OtaConfig>::decode(JsonObject& json);