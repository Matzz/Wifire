#pragma once

#include <SmingCore.h>
#include "../Configuration/Users/UsersConfigProvider.h"
#include "../Utils/Either.h"

class Session {
public:
    String login;
    String sessionId;
	Vector<String> roles;
    uint64_t lastUsed;
    Session();
    Session(String login, String sessionId, const Vector<String> &roles, uint64_t lastUsed = 0);
    Session& operator=(const Session &session);
    
    inline void markUsed() {
        lastUsed = os_get_nanoseconds();
    }
};

class UserSessionManager {
public:
	UserSessionManager(UsersConfigProvider& configProvider);
    Either<String, Session> validateSession(String sessionId);
    Either<String, Session> signin(String login, String password);
private:
    Vector<Session> sessions;
    UsersConfigProvider& configProvider;

    int getSessionByLogin(String login);
    int getSessionById(String sessionId);
    String mkSessionId(String login);
};