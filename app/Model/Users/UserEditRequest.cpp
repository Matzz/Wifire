#include "UserEditRequest.h"
#include <SmingCore.h>
#include "../StringVectorCodec.h"

template<>
void Codec<UserEditRequest>::encode(JsonObject& json, const UserEditRequest &userToEdit) {
    json["enabled"] = userToEdit.enabled;
    json["login"] = userToEdit.login;
    json["password"] = userToEdit.password;
    StringVectorCodec::encode(json, userToEdit.roles, "roles");
}

template<>
Either<String, UserEditRequest> Codec<UserEditRequest>::decode(JsonObject& json) {
    UserEditRequest cfg {
        json["enabled"].as<bool>(),
        json["login"].as<String>(),
        json["password"].as<String>(),
        StringVectorCodec::decode(json,  "roles")
    };
    return {RightTagT(), std::move(cfg)};
}