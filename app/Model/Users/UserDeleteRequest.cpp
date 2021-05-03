#include "UserDeleteRequest.h"
#include <SmingCore.h>

template<>
void Codec<UserDeleteRequest>::encode(JsonObject& json, const UserDeleteRequest &req) {
	json["login"] = req.login;
}

template<>
Either<String, UserDeleteRequest> Codec<UserDeleteRequest>::decode(JsonObject& json) {
	UserDeleteRequest cfg;
	cfg.login = json["login"].as<String>();
	return {RightTagT(), std::move(cfg)};
}