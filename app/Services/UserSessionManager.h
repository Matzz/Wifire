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
        lastUsed = micros();
    }
};

class UserSessionManager {
public:
	UserSessionManager(UsersConfigProvider& configProvider);
    Either<String, Session> validateSession(const String& sessionId);
    Either<String, Session> signIn(const String& login, const String& password);\
    void signOut(const String& sessionId);
private:
    Vector<Session> sessions;
    UsersConfigProvider& configProvider;

    int getSessionByLogin(const String& login);
    int getSessionById(const String& sessionId);
    String mkSessionId(const String& login);
};