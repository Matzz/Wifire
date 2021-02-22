#include "SpiffsManager.h"


Storage::Partition SpiffsManager::findSpiffsPartition(uint8_t slot)
{
	String name = F("spiffs");
	name += slot;
	auto part = Storage::findPartition(name);
	if(!part) {
		debug_w("Partition '%s' not found", name.c_str());
	}
	return part;
}

void SpiffsManager::setup() {
	auto partition = SpiffsManager::findSpiffsPartition();

	debug_i("Mouting spiffs partition %s (%d).\n", partition.name().c_str(), partition.address());
	spiffs_mount(partition);
}
