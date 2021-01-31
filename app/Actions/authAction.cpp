#include "authAction.h"
#include "../Services/Injector.h"
#include "../Utils/utils.h"

const String sessionIdField = "sessionId";

void signInAction(HttpRequest &request, HttpResponse &response) {
	UserSessionManager& sessionManager = Injector::getInstance().getUserSessionManager();

    auto oldSid = getSessionId(request);
    if(oldSid != String::empty) {
        sessionManager.signOut(oldSid);
    }

    String login = getString(request, "login");
    String password = getString(request, "password");
    debug_i("L: %s, P: %s", login, password);
    Either<String, Session> sessionOrErr = sessionManager.signIn(login, password);
    auto session = sessionOrErr.get_if_right();
    if(session != nullptr) {
        JsonObjectStream* stream = new JsonObjectStream();
        JsonObject json = stream->getRoot();
        json["login"] = login;
        JsonArray rolesArr = json.createNestedArray("roles");
        for(int j=0; j<session->roles.size(); j++) {
		    rolesArr.add(session->roles[j]);
        }
        String authJson = stream->readString(2048);

        response.setCookie("auth", authJson);
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
    response.setCookie(sessionIdField, String::empty);
}

void setSessionId(HttpResponse &response, const String& sessionId) {
    response.setCookie(sessionIdField, sessionId);
}

const String getSessionId(HttpRequest& request) {
    String cookieStr = request.getHeader("Cookie");
    Vector<String> cookiesKV;
    splitString(cookieStr, ';', cookiesKV);
    for(int i=0; i<cookiesKV.size(); i++) {
        String kv = cookiesKV[i];
        kv.trim();
        if(kv.startsWith(sessionIdField + "=")) {
            kv.remove(0, sessionIdField.length() + 1);
            kv.trim();
            return kv;
        } else {
            return String::empty;
        }
    }
    return String::empty;
}