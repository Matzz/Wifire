#pragma once

#include <SmingCore.h>
#include "../Codec.h"
#include "../StringVectorCodec.h"

class UserEditRequest {
public:
    bool enabled;
	String login;
	String password;
	Vector<String> roles;

    UserEditRequest(bool enabled, String login, String password, const Vector<String> &roles);
};

template<> class Codec<UserEditRequest> {
	public:
        static Codec<UserEditRequest>& getInstance() {
            static Codec<UserEditRequest> instance;
            return instance;
        }

	void encode(JsonObject& json, UserEditRequest userToEdit) {
        json["enabled"] = userToEdit.enabled;
        json["login"] = userToEdit.login;
        json["password"] = userToEdit.password;
        StringVectorCodec::encode(json, userToEdit.roles, "roles");
	}

	Either<String, UserEditRequest> decode(JsonObject& json) {
		UserEditRequest cfg(
            json["enabled"].as<bool>(),
            json["login"].as<String>(),
            json["password"].as<String>(),
            StringVectorCodec::decode(json,  "roles")
        );
		return {RightTagT(), std::move(cfg)};
	}
};