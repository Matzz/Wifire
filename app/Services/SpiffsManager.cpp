#include "SpiffsManager.h"
#include "Injector.h"

void SpiffsManager::setup() {
	Injector di = Injector::getInstance();
	int slot = rboot_get_current_rom();

	Serial.println();
	Serial.printf("Currently running rom %d.\n", slot);
	int address = di.spiffsAddresses[slot];

	Serial.printf("trying to mount spiffs at %x, length %d\n", address, di.spiffsSize);

	spiffs_mount_manual(address, di.spiffsSize);
	Serial.println();
}
