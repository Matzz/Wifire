#ifndef APP_ACTIONS_OTAUPDATE_H_
#define APP_ACTIONS_OTAUPDATE_H_
#include <SmingCore/SmingCore.h>
#include "../Configuration/OtaConfigProvider.h"

class OtaUpdater {
	protected:
		int spiffsAddresses[2];
		OtaConfigProvider& cfgProvider;

		static uint8 activeSlot();
		static uint8 inactiveSlot();
		static void callback(rBootHttpUpdate& client, bool result);

	public:
		OtaUpdater(const int spiffsAddresses[2],
					OtaConfigProvider &cfgProvider);
		void update();
		static void switchSlot();
};

#endif
