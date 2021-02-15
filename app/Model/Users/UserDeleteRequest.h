#pragma once
#include <SmingCore.h>
#include "../Codec.h"

class UserDeleteRequest {
public:
	String login;
};

template<> class Codec<UserDeleteRequest> {
public:
    static Codec<UserDeleteRequest>& getInstance() {
        static Codec<UserDeleteRequest> instance;
        return instance;
    }

	void encode(JsonObject& json, UserDeleteRequest req) {
        json["login"] = req.login;
	}

	Either<String, UserDeleteRequest> decode(JsonObject& json) {
		UserDeleteRequest cfg;
		cfg.login = json["login"].as<String>();
		return {RightTagT(), std::move(cfg)};
	}
};