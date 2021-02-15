#include "OtaUpdater.h"

#include <SmingCore.h>

uint8 OtaUpdater::activeSlot() {
	return rboot_get_current_rom();
}

uint8 OtaUpdater::inactiveSlot() {
	return !activeSlot();
}

void OtaUpdater::callback(RbootHttpUpdater& client, bool result) {
	debug_i("OTA - In callback...");
	if (result) {
		debug_i("OTA - Firmware updated.");
		switchSlot();
	} else {
		// fail
		debug_e("OTA - Firmware update failed!");
	}
}

OtaUpdater::OtaUpdater(const int spiffsAddresses[2],
		ConfigProvider<OtaConfig> &cfgProvider) :
		spiffsAddresses { spiffsAddresses[0], spiffsAddresses[1] }, cfgProvider(
				cfgProvider) {
}

void OtaUpdater::update() {
	auto configOrError = cfgProvider.load();
	if(configOrError.isLeft()) {
		return;
	}
	auto config = *configOrError.getIfRight();

	debug_i("OTA - Updating...");
	RbootHttpUpdater* otaUpdater = new RbootHttpUpdater();
	uint8 slot = activeSlot();

	// flash rom to position indicated in the rBoot config rom table
	rboot_config boot_config = rboot_get_config();
	otaUpdater->addItem(boot_config.roms[slot], config.romUrl);
	otaUpdater->addItem(spiffsAddresses[slot], config.spiffUrl);

	// request switch and reboot on success
	//otaUpdater->switchToRom(slot);
	// and/or set a callback (called on failure or success without switching requested)
	otaUpdater->setCallback(callback);

	// start update
	otaUpdater->start();
	delete otaUpdater;
}

void OtaUpdater::switchSlot() {
	uint8 currentSlot = activeSlot();
	uint8 newSlot = inactiveSlot();
	Serial.printf("SOTA - wapping from rom %d to rom %d.\n", currentSlot, newSlot);
	if (!rboot_set_current_rom(newSlot)) {
		debug_e("OTA - Setting slot failed!");
	}
	debug_i("OTA - Restarting...\n");
	System.restart();
}
