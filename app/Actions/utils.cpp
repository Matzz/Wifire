#include "utils.h"

bool getBool(HttpRequest &request, String name) {
	String val = request.getPostParameter(name);
	return val=="true" || val == "1";
}