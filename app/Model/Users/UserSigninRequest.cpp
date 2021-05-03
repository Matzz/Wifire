#include "UserSigninRequest.h"
#include <SmingCore.h>

template<>
void Codec<UserSigninRequest>::encode(JsonObject& json, const UserSigninRequest &user) {
	json["login"] = user.login;
	json["password"] = user.password;
}

template<>
Either<String, UserSigninRequest> Codec<UserSigninRequest>::decode(JsonObject& json) {
	UserSigninRequest cfg;
	cfg.login = json["login"].as<String>();
	cfg.password = json["password"].as<String>();
	return {RightTagT(), std::move(cfg)};
}