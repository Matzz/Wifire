#pragma once

#include <SmingCore.h>

class AvaiableNetworksProvider {
public:
	static BssList networks;
	static void startScan();
	static void networkScanCompleted(bool succeeded, BssList list);
	static bool isScanning() { return scanning; }
	static bool wasEverScanned() { return lastScanTime > 0; }
	static unsigned long lastScanAgeSeconds() { return (millis() - lastScanTime)/1000; }
private:
	static bool scanning;
	static unsigned long lastScanTime;
};