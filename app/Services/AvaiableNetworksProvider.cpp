#include "AvaiableNetworksProvider.h"

void AvaiableNetworksProvider::startScan() {
	WifiStation.startScan(networkScanCompleted);
}
void AvaiableNetworksProvider::networkScanCompleted(bool succeeded, BssList list) {
	networks.clear();
	if (succeeded) {
		for (int i = 0; i < list.count(); i++)
			if (!list[i].hidden && list[i].ssid.length() > 0)
				networks.add(list[i]);
	} else {
		startScan();
	}
}
