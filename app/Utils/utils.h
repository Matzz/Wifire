#pragma once

#include <SmingCore.h>
#include <Crypto/Sha1.h>
#include <JsonObjectStream.h>

bool getBool(HttpRequest& request, String name);
String getString(HttpRequest& request, String name, String defaultVal = "");

void returnFailure(HttpResponse &response, String msg);

String getHash(String base);

const String getSessionId(HttpRequest& request);