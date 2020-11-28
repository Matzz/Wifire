#include "utils.h"

bool getBool(HttpRequest &request, String name) {
	String val = request.getPostParameter(name);
	return val=="true" || val == "1" || val == "on";
}

String getHash(String base) {
		auto hash = Crypto::Sha1().calculate(base);
		return Crypto::toString(hash);
}