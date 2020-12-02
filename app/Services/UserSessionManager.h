#pragma once

#include <SmingCore.h>
#include "../Configuration/Users/UsersConfigProvider.h"
#include "../Utils/Either.h"

class Session {
public:
    String login;
    String sessionId;
    Vector<String> roles;
    Session(String login, String sessionId, Vector<String> roles);
};

class UserSessionManager {
public:
	UserSessionManager(UsersConfigProvider& configProvider);
    Either<String, Session> signin(String login, String password);
private:
    UsersConfigProvider& configProvider;
    String mkSessionId(String login);
};