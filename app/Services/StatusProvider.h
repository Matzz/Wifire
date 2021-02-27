#pragma once

#include <SmingCore.h>
#include <Wiring/WHashMap.h>

class StatusProvider {
public:
	static HashMap<String, String>* getStatus(bool showPassword);
};