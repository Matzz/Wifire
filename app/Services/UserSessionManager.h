#pragma once

#include <SmingCore.h>
#include "../Configuration/Users/UsersConfig.h"
#include "../Configuration/ConfigProvider.h"
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
    static void setSessionCookie(HttpResponse& response, String cookie);
    static void clearSessionCookie(HttpResponse& response);
    
	UserSessionManager(ConfigProvider<UsersConfig>& configProvider);
    Either<String, Session> validateSession(const String& sessionId);
    Either<String, Session> signIn(const String& login, const String& password);
    void signOut(const String& sessionId);
    void signOutByLogin(const String& login);

private:
    Vector<Session> sessions;
    ConfigProvider<UsersConfig>& configProvider;

    int getSessionByLogin(const String& login);
    int getSessionById(const String& sessionId);
    String mkSessionId(const String& login);
};