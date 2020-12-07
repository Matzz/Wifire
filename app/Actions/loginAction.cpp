#include "loginAction.h"
#include "../Services/Injector.h"
#include "../Utils/utils.h"

void signinAction(HttpRequest &request, HttpResponse &response) {

	UserSessionManager& sessionManager = Injector::getInstance().getUserSessionManager();
    String login = getString(request, "login");
    String password = getString(request, "password");
    Either<String, Session> maybeSession = sessionManager.signin(login, password);

    // JsonObjectStream* stream = new JsonObjectStream();
    // JsonObject json = stream->getRoot();
    // Session session = Session();
	// if(maybeSession.matchRight(session)) {
    //     json["sessionId"] = session.sessionId;
		
	// } else {
    //     String error;
	// 	maybeSession.matchLeft(error);
    //     returnFailure(response, json, error);
	// }
        
    // response.sendNamedStream(stream);
}