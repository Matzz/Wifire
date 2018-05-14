#ifndef APP_AVAIABLE_NETWORKS_PROVIDER
#define APP_AVAIABLE_NETWORKS_PROVIDER

#include <SmingCore/SmingCore.h>

class AvaiableNetworksProvider {
public:
	static BssList networks;
	static void startScan();
	static void networkScanCompleted(bool succeeded, BssList list);
};

#endif
