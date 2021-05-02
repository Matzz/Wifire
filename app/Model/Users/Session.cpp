#include "Session.h"

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