#pragma once

#include <SmingCore.h>
#include <Network/RbootHttpUpdater.h>
#include "../Configuration/ConfigProvider.h"
#include "../Configuration/OtaConfig.h"

class OtaUpdater {
	protected:
		int spiffsAddresses[2];
		ConfigProvider<OtaConfig>& cfgProvider;

		static uint8 activeSlot();
		static uint8 inactiveSlot();
		static void callback(RbootHttpUpdater& client, bool result);

	public:
		OtaUpdater(const int spiffsAddresses[2],
					ConfigProvider<OtaConfig> &cfgProvider);
		void update();
		static void switchSlot();
};
