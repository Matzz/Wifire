#include "UserSessionManager.h"

Session::Session():
    login(""), sessionId(""), roles(Vector<String>(1, 1)), lastUsed(0) { }

Session::Session(String login, String sessionId, const Vector<String> &roles, uint64_t lastUsed):
    login(login), sessionId(sessionId), roles(roles), lastUsed(lastUsed) { }

Session& Session::operator=(const Session &session) {
        login = session.login;
        sessionId = session.sessionId;
        roles = session.roles;
        lastUsed = session.lastUsed;
        return *this;
}

void UserSessionManager::setSessionCookie(HttpResponse& response, const String cookie) {
        response.setCookie("auth", cookie + "; Path=/");
}

void UserSessionManager::clearSessionCookie(HttpResponse& response) {
        response.setCookie("auth", "; Path=/");
}

UserSessionManager::UserSessionManager(ConfigProvider<UsersConfig>& configProvider) :
		configProvider(configProvider) { }

int UserSessionManager::getSessionByLogin(const String& login) {
    for(int i=0; i<sessions.size(); i++) {
        Session session = sessions[i];
        if(session.login==login) {
            return i;
        }
    }
    return -1;
}

int UserSessionManager::getSessionById(const String& sessionId) {
    for(int i=0; i<sessions.size(); i++) {
        Session session = sessions[i];
        if(session.sessionId==sessionId) {
            return i;
        }
    }
    return -1;
}

Either<String, Session> UserSessionManager::validateSession(const String& sessionId) {
    int sessionIdx = getSessionById(sessionId);
    if(sessionIdx >= 0) {
        sessions[sessionIdx].markUsed();
        return {RightTagT(), std::move(sessions[sessionIdx])};
    } else {
        return {LeftTagT(), std::move(F("Session expired"))};
    }

}

Either<String, Session> UserSessionManager::signIn(const String& login, const String& password) {

	auto configOrError = configProvider.load();
	if(configOrError.isLeft()) {
		return {LeftTagT(), *configOrError.getIfLeft()};
	}
	auto usersConfig = *configOrError.getIfRight();
    UserConfig const * user = usersConfig.getUser(login);

    if(user != nullptr) {
        if(!user->enabled) {
            return {LeftTagT(), std::move(F("This user is disabled."))};
        }
        if(!user->checkPassword(password)) {
            return {LeftTagT(), std::move("Invalid password.")};
        }
        int sessionIdx = getSessionByLogin(login);

        if(sessionIdx < 0) {
            sessions.addElement(Session(login, mkSessionId(login), user->roles, 0));
            sessionIdx = sessions.size() -1;
        }
        sessions[sessionIdx].markUsed();
        return {RightTagT(), std::move(sessions[sessionIdx])};

    } else {
        return {LeftTagT(), std::move("User not found.")};
    }
}

void UserSessionManager::signOut(const String& sessionId) {
    int idx = getSessionById(sessionId);
    if(idx >= 0) {
        sessions.removeElementAt(idx);
    }
}

void UserSessionManager::signOutByLogin(const String& login) {
    int idx = getSessionByLogin(login);
    if(idx >= 0) {
        sessions.removeElementAt(idx);
    }
}

String UserSessionManager::mkSessionId(const String& login) {
    auto base = String(system_get_chip_id(), 10) + String(micros(), 10) + String(login);
    return UserConfig::getHash(base);
}