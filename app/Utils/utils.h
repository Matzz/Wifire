#pragma once

#include <SmingCore.h>
#include <Crypto/Sha1.h>

bool getBool(const HttpRequest &request, const String name);
String getString(const HttpRequest &request, const String name, const String defaultVal = "");

String getHash(String base);