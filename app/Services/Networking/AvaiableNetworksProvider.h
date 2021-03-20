#pragma once

#include <SmingCore.h>
#include "../../Utils/NonCopyable.h"
#include "WiFiManager.h"

class AvaiableNetworksProvider : private NonCopyable {
public:
	AvaiableNetworksProvider(WiFiManager& wiFiManager);
	void startScan();
	bool isScanning() { return scanning; }
	bool wasEverScanned() { return lastScanTime > 0; }
	unsigned long lastScanAgeSeconds() { return (millis() - lastScanTime)/1000; }
	const BssList getNetworks() { return networks; }

	WiFiManager& wiFiManager;

	BssList networks = BssList();
	bool scanning = false;
	unsigned long lastScanTime = 0;
	static void networkScanCompleted(bool succeeded, BssList list);
};