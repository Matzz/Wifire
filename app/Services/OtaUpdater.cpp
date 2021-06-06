#include "OtaUpdater.h"

#include <SmingCore.h>
#include "SpiffsManager.h"

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

OtaUpdater::OtaUpdater(ConfigProvider<OtaConfig> &cfgProvider) : cfgProvider(cfgProvider) { }

void OtaUpdater::update() {
	auto configOrError = cfgProvider.load();
	if(configOrError.isLeft()) {
		return;
	}
	auto config = *configOrError.getIfRight();

	uint8 slot = activeSlot();
	debug_i("OTA - Updating slot %d", slot);
	RbootHttpUpdater* otaUpdater = new RbootHttpUpdater();

	// flash rom to position indicated in the rBoot config rom table
	rboot_config boot_config = rboot_get_config();
	
	debug_i("Addig romUrl %d = %s", boot_config.roms[slot], config.romUrl.c_str());
	otaUpdater->addItem(boot_config.roms[slot], config.romUrl);

	auto part = SpiffsManager::findSpiffsPartition(slot);
	if(part) {
		debug_i("Addig spiffUrl %d = %s", part.address(), config.spiffUrl.c_str());
		// use user supplied values (defaults for 4mb flash in hardware config)
		otaUpdater->addItem(part.address(), config.spiffUrl, part.size());
	} else {
		debug_i("Not found spiff partition.");
	}
	otaUpdater->setCallback(callback);

	debug_i("OTA - Start upgrade...");
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
