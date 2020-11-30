#pragma once

#include <SmingCore.h>
#include <Crypto/Sha1.h>

bool getBool(HttpRequest& request, String name);
String getString(HttpRequest& request, String name, String defaultVal = "");

String getHash(String base);