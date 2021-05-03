#pragma once

#include "../Codec.h"

class UserEditRequest {
public:
    bool enabled;
	String login;
	String password;
	Vector<String> roles;
};

template<>
void Codec<UserEditRequest>::encode(JsonObject& json, const UserEditRequest &userToEdit);

template<>
Either<String, UserEditRequest> Codec<UserEditRequest>::decode(JsonObject& json);