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


	debug_i("trying to mount '%s' at 0x%08x, length %d", partition.name().c_str(), partition.address(),
			partition.size());
	spiffs_mount(partition);
}
