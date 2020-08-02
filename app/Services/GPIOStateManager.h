#pragma once
#include <SmingCore.h>
#include <Wiring/WHashMap.h>
#include "../Configuration/GPIOConfigProvider.h"
#include "../Utils/NonCopyable.h"

class GPIOStateManager : private NonCopyable {
	protected:
		GPIOConfigProvider& cfgProvider;
		Timer** timers = new Timer*[PIN_MAX+1];
		void deleteTimer(int pin);
		int getPinByName(GPIOConfig &config, String name);
	public:
		GPIOStateManager(GPIOConfigProvider &cfgProvider);
		~GPIOStateManager();
		void update();
		bool turnOn(String pin, unsigned int howLong = 0);
};