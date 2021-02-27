#include "authAction.h"
#include "../Services/Injector.h"
#include "../Model/StringVectorCodec.h"
#include "actionsHelpers.h"


void signInAction(HttpRequest &request, HttpResponse &response) {
	UserSessionManager& sessionManager = Injector::getInstance().getUserSessionManager();

    auto oldSid = getSessionId(request);
    if(oldSid != String::empty) {
        sessionManager.signOut(oldSid);
    }

	Either<String, UserSigninRequest> configOrError = decodeJson<UserSigninRequest>(request);
	if(configOrError.isLeft()) {
		return returnFailure(response, *configOrError.getIfLeft());
	}
	UserSigninRequest* signinData = configOrError.getIfRight();

    Either<String, Session> sessionOrErr = sessionManager.signIn(signinData->login, signinData->password);
    auto session = sessionOrErr.getIfRight();
    if(session != nullptr) {

		DynamicJsonDocument doc(JSON_MAX_SIZE);
        JsonObject json = doc.to<JsonObject>();
        json["login"] = signinData->login;
        json["sessionId"] = session->sessionId;
        StringVectorCodec::encode(json, session->roles, "roles");
        String authJson;
        serializeJson(json, authJson);
        UserSessionManager::setSessionCookie(response, authJson);
        response.setContentType(MIME_JSON);
        response.sendString(authJson);

    } else {
		  returnFailure(response, *sessionOrErr.getIfLeft());
          return;
    }
}

void signOutAction(HttpRequest &request, HttpResponse &response) {
	UserSessionManager& sessionManager = Injector::getInstance().getUserSessionManager();
    auto sessionId = getSessionId(request);
    sessionManager.signOut(sessionId);
    UserSessionManager::clearSessionCookie(response);
    returnOk(response, "");
}