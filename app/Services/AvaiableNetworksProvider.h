#pragma once

#include <SmingCore.h>

class AvaiableNetworksProvider {
public:
	static BssList networks;
	static void startScan();
	static void networkScanCompleted(bool succeeded, BssList list);
	static bool isScanning() { return scanning; }
private:
	static bool scanning;
};
