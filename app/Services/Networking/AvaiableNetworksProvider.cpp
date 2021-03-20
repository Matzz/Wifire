#include "AvaiableNetworksProvider.h"
#include "../Injector.h"

AvaiableNetworksProvider::AvaiableNetworksProvider(WiFiManager& wiFiManager) :
	wiFiManager(wiFiManager) { }

void AvaiableNetworksProvider::startScan() {
	if(!scanning) {
		wiFiManager.startTempStationMode();
		Serial.printf("Pointer 2: %p\n", this);
		scanning = WifiStation.startScan(AvaiableNetworksProvider::networkScanCompleted);
		lastScanTime = micros();
	}
}

void AvaiableNetworksProvider::networkScanCompleted(bool succeeded, BssList list) {
	// binding using std:bind cause and memory errors
	AvaiableNetworksProvider& provider = Injector::getInstance().getAvaiableNetworksProvider();
	provider.scanning = false;
	if (succeeded) {
		provider.networks = list;
		// TODO TEST BELOW
		// provider.wiFiManager.stopTempStationMode();
	} else {
		provider.startScan();
	}
}
