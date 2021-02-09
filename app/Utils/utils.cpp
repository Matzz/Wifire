#include "utils.h"

bool getBool(HttpRequest& request, String name) {
	String val = request.getPostParameter(name);
	return val=="true" || val == "1" || val == "on";
}

void returnFailure(HttpResponse &response, String msg) {
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	json["message"] = msg;
	response.sendDataStream(stream, MIME_JSON);
	response.code = HttpStatus::BAD_REQUEST;
}

String getString(HttpRequest& request, String name, String defaultVal) {
	String maybeParam = request.getPostParameter(name);
	if(maybeParam == null) {
		return defaultVal;
	} else {
		return maybeParam;
	}
}

String getHash(String base) {
		auto hash = Crypto::Sha1().calculate(base);
		return Crypto::toString(hash);
}

const String getCookie(HttpRequest& request, String name) {
	String cookiesStr = request.getHeader("Cookie");
	int startPos = cookiesStr.indexOf("auth");
	if(startPos < 0) {
		return String::empty;
	}
	startPos += 5; // "auth="
	int endPos = cookiesStr.indexOf(";", startPos);
	if(endPos < 0) {
		endPos = cookiesStr.length();
	}
	return cookiesStr.substring(startPos, endPos);
}

const String getSessionId(HttpRequest& request) {
    String cookieStr = getCookie(request, "auth");
    if(cookieStr != String::empty) {
        StaticJsonDocument<512> doc;
        DeserializationError err = deserializeJson(doc, cookieStr);
        if(err) {
			return String::empty;
        } else if(doc.containsKey("sessionId")) {
			String sId = doc["sessionId"];
            return sId;
        } else {
			return String::empty;
        }
    } else {
		return String::empty;
	}
}