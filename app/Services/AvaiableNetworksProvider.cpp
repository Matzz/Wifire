#include "AvaiableNetworksProvider.h"

bool AvaiableNetworksProvider::scanning = false;
BssList AvaiableNetworksProvider::networks;

void AvaiableNetworksProvider::startScan() {
	if(!scanning) {
		scanning = true;
		WifiStation.startScan(networkScanCompleted);
	}
}
void AvaiableNetworksProvider::networkScanCompleted(bool succeeded, BssList list) {
	networks.clear();
	scanning = false;
	if (succeeded) {
		for (int i = 0; i < list.count(); i++)
			if (!list[i].hidden && list[i].ssid.length() > 0)
				networks.add(list[i]);
	} else {
		startScan();
	}
}
