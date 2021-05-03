#pragma once
#include "../Codec.h"

class UserSigninRequest {
public:
	String login;
	String password;
};

template<>
void Codec<UserSigninRequest>::encode(JsonObject& json, const UserSigninRequest &user);

template<>
Either<String, UserSigninRequest> Codec<UserSigninRequest>::decode(JsonObject& json);