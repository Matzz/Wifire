#include "OtaConfig.h"

template<>
void Codec<OtaConfig>::encode(JsonObject& json, const OtaConfig &cfg) {
	json["romUrl"] = cfg.romUrl;
	json["spiffUrl"] = cfg.spiffUrl;
}

template<>
Either<String, OtaConfig> Codec<OtaConfig>::decode(JsonObject& json) {
	OtaConfig cfg;
	cfg.romUrl = CodecHelpers::getOrElse(json, "romUrl", String::empty);
	cfg.spiffUrl = CodecHelpers::getOrElse(json, "spiffUrl", String::empty);
	return cfg;
}