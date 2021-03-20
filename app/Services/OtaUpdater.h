#pragma once

#include <SmingCore.h>
#include <Network/RbootHttpUpdater.h>
#include "ConfigProvider.h"
#include "../Utils/NonCopyable.h"
#include "../Model/OtaConfig.h"

class OtaUpdater : private NonCopyable {
	protected:
		ConfigProvider<OtaConfig>& cfgProvider;

		static uint8 activeSlot();
		static uint8 inactiveSlot();
		static void callback(RbootHttpUpdater& client, bool result);

	public:
		OtaUpdater(ConfigProvider<OtaConfig> &cfgProvider);
		void update();
		static void switchSlot();
};
