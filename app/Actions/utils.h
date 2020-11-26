#pragma once

#include <SmingCore.h>
#include <Crypto/Sha1.h>

bool getBool(HttpRequest &request, String name);

String getHash(String base) {
		auto hash = Crypto::Sha1().calculate(base);
		return Crypto::toString(hash);
}