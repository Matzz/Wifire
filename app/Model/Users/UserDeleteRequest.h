#pragma once
#include <SmingCore.h>
#include "../Codec.h"

class UserDeleteRequest {
public:
	String login;
};

template<>
void Codec<UserDeleteRequest>::encode(JsonObject& json, const UserDeleteRequest &req);

template<>
Either<String, UserDeleteRequest> Codec<UserDeleteRequest>::decode(JsonObject& json);