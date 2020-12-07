#include "UserSessionManager.h"
#include "../Utils/utils.h"


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

UserSessionManager::UserSessionManager(UsersConfigProvider& configProvider) :
		configProvider(configProvider) { }

int UserSessionManager::getSessionByLogin(String login) {
    for(int i=0; i<sessions.size(); i++) {
        Session session = sessions[i];
        if(session.login==login) {
            return i;
        }
    }
    return -1;
}

int UserSessionManager::getSessionById(String sessionId) {
    for(int i=0; i<sessions.size(); i++) {
        Session session = sessions[i];
        if(session.sessionId==sessionId) {
            return i;
        }
    }
    return -1;
}

Either<String, Session> UserSessionManager::validateSession(String sessionId) {
    int sessionIdx = getSessionById(sessionId);
    if(sessionIdx >= 0) {
        sessions[sessionIdx].markUsed();
        return {right_tag_t(), std::move(sessions[sessionIdx])};
    } else {
        return {left_tag_t(), std::move("Session expired")};
    }

}

Either<String, Session> UserSessionManager::signin(String login, String password) {
    UsersConfig usersConfig = configProvider.load();
    const UserConfig* user = usersConfig.getUser(login);
    if(user != nullptr) {
        if(user->enabled) {
            return {left_tag_t(), std::move("This user is disabled.")};
        }
        if(!user->checkPassword(password)) {
            return {left_tag_t(), std::move("Invalid password.")};
        }
        int sessionIdx = getSessionByLogin(login);

        if(sessionIdx < 0) {
            sessions.addElement(Session(login, mkSessionId(login), user->roles, 0));
            sessionIdx = sessions.size() -1;
        }
        sessions[sessionIdx].markUsed();
        return {right_tag_t(), std::move(sessions[sessionIdx])};

    } else {
        return {left_tag_t(), std::move("User not found.")};
    }
}

String UserSessionManager::mkSessionId(String login) {
    auto base = String(system_get_chip_id(), 10) + String(os_get_nanoseconds(), 10) + String(login);
    return getHash(base);
}