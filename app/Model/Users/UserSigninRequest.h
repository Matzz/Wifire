#pragma once
#include <SmingCore.h>
#include "../Codec.h"

class UserSigninRequest {
public:
	String login;
	String password;
};

template<> class Codec<UserSigninRequest> {
public:
    static Codec<UserSigninRequest>& getInstance() {
        static Codec<UserSigninRequest> instance;
        return instance;
    }

	void encode(JsonObject& json, const UserSigninRequest &user) {
        json["login"] = user.login;
        json["password"] = user.password;
	}

	Either<String, UserSigninRequest> decode(JsonObject& json) {
		UserSigninRequest cfg;
		cfg.login = json["login"].as<String>();
		cfg.password = json["password"].as<String>();
		return {RightTagT(), std::move(cfg)};
	}
};