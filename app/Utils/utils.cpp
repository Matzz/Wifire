#include "utils.h"

bool getBool(HttpRequest &request, const String& name) {
	String val = request.getPostParameter(name);
	return val=="true" || val == "1" || val == "on";
}
String getString(HttpRequest &request, const String& name, const String& defaultVal) {
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