#pragma once

#include <SmingCore.h>
#include "ConfigProvider.h"
#include "../Model/Users/UsersConfig.h"
#include "../Model/Users/Session.h"
#include "../Utils/NonCopyable.h"
#include "../Utils/Either.h"


class SessionsConfigProvider : public FileConfigProvider<Vector<Session>> {
    protected:
    ConfigProvider<UsersConfig>& usersProvider;

    public:
	SessionsConfigProvider(String fileName, ConfigProvider<UsersConfig>& usersProvider);
	Either<String, Vector<Session>> load();

};

class UserSessionManager : private NonCopyable {
public:
    static void setSessionCookie(HttpResponse& response, String cookie);
    static void clearSessionCookie(HttpResponse& response);
    
	UserSessionManager(ConfigProvider<UsersConfig>& userProvider, ConfigProvider<Vector<Session>>& sessionProvider);
    Either<String, Session> validateSession(const String& sessionId);
    Either<String, Session> signIn(const String& login, const String& password);
    void signOut(const String& sessionId);
    void signOutByLogin(const String& login);

private:
    Vector<Session> sessions;
    ConfigProvider<UsersConfig>& userProvider;
    ConfigProvider<Vector<Session>>& sessionProvider;

    int getSessionByLogin(const String& login);
    int getSessionById(const String& sessionId);
    String mkSessionId(const String& login);
};