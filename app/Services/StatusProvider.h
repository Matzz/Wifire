#pragma once

#include <SmingCore.h>
#include "../Model/Status.h"

class StatusProvider {
public:
	static Status getStatus(bool showPassword);
};