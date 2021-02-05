#include "authAction.h"
#include "../Services/Injector.h"
#include "../Utils/utils.h"

void signInAction(HttpRequest &request, HttpResponse &response) {
	UserSessionManager& sessionManager = Injector::getInstance().getUserSessionManager();

    auto oldSid = getSessionId(request);
    if(oldSid != String::empty) {
        sessionManager.signOut(oldSid);
    }

    String login = getString(request, "login");
    String password = getString(request, "password");
    Either<String, Session> sessionOrErr = sessionManager.signIn(login, password);
    auto session = sessionOrErr.get_if_right();
    if(session != nullptr) {
        JsonObjectStream* stream = new JsonObjectStream();
        JsonObject json = stream->getRoot();
        json["login"] = login;
        json["sessionId"] = session->sessionId;
        JsonArray rolesArr = json.createNestedArray("roles");
        for(int j=0; j<session->roles.size(); j++) {
		    rolesArr.add(session->roles[j]);
        }
        String authJson = stream->readString(2048);
        UserSessionManager::setSessionCookie(response, authJson);
        response.setContentType(MIME_JSON);
        response.sendString(authJson);

    } else {
		  returnFailure(response, *sessionOrErr.get_if_left());
          return;
    }
}

void signOutAction(HttpRequest &request, HttpResponse &response) {
	UserSessionManager& sessionManager = Injector::getInstance().getUserSessionManager();
    auto sessionId = getSessionId(request);
    sessionManager.signOut(sessionId);
    UserSessionManager::clearSessionCookie(response);
}