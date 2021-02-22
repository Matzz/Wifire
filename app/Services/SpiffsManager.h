#pragma once
#include <SmingCore.h>
#include <rboot-api.h>

class SpiffsManager {
public:
	static void setup();
	static Storage::Partition findSpiffsPartition(uint8_t slot = rboot_get_current_rom());
};
