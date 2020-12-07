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