#pragma once
#include <SmingCore.h>
#include <Wiring/WHashMap.h>
#include "ConfigProvider.h"
#include "../Model/GPIOConfig.h"
#include "../Utils/NonCopyable.h"

class GPIOStateManager : private NonCopyable {
	protected:
		ConfigProvider<GPIOConfig>& cfgProvider;
		Timer** timers = new Timer*[GPIOConfig::max_pin+1];
		void deleteTimer(int pin);
		int getPinByName(GPIOConfig &config, String name);
	public:
		GPIOStateManager(ConfigProvider<GPIOConfig>& cfgProvider);
		~GPIOStateManager();
		void update();
		bool switchPin(String pinName, uint8_t state, uint32_t howLongMs = 0);
};