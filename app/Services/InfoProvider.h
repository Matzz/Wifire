#pragma once

#include <SmingCore.h>
#include <Wiring/WHashMap.h>

class InfoProvider {
public:
	static HashMap<String, String>* getInfo(bool showPassword);
};