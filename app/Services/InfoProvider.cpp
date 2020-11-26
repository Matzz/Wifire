#include "InfoProvider.h"

#include <SmingCore.h>
#include <WHashMap.h>
#include <esp_spi_flash.h>
#ifdef ENABLE_MALLOC_COUNT
#include <malloc_count.h>
#endif

HashMap<String, String>* InfoProvider::getInfo(bool showPassword)  {
	auto map = new HashMap<String, String>;
	(*map)["sdk"] = system_get_sdk_version();
	auto heep_free = system_get_free_heap_size();
	(*map)["heep_current_free_memory"] = String(heep_free, 10);


#ifdef ENABLE_MALLOC_COUNT
	auto heep_current = MallocCount::getCurrent();
	auto heep_size = heep_current+heep_free;
	float heep_perc = heep_current;
	heep_perc = heep_perc/heep_size*100;
	(*map)["heep_current_memory_allocated"] = heep_current;
	(*map)["heep_usage_percentage"] = heep_perc;
	(*map)["heep_peak_memory_allocated"] = MallocCount::getPeak();
	(*map)["heep_number_of_allocations_called"] = MallocCount::getAllocCount();
	(*map)["heep_cumulative_memory_allocated"] = MallocCount::getTotal();
#endif

	(*map)["cpu frequency"] = String(system_get_cpu_freq(), 10);
	(*map)["chip id"] = String(system_get_chip_id(), 10);
(*map)["flash id"] = String(spi_flash_get_id(), 10);

	(*map)["wifi station enabled"] = WifiStation.isEnabled() ? "true" : "false";
	(*map)["wifi station channel"] = String(WifiStation.getChannel(), 10);
	(*map)["wifi station is connected"] = WifiStation.isConnected() ? "true" : "false";
	(*map)["wifi station is connection failed"] = WifiStation.isConnectionFailed() ? "true" : "false";
	(*map)["wifi station DHCP enaled"] = WifiStation.isEnabledDHCP() ? "true" : "false";
	(*map)["wifi station connection status"] = WifiStation.getConnectionStatusName();
	String hostName = WifiStation.getHostname();
	(*map)["wifi station hostname"] = hostName != NULL ? hostName : "null";
	(*map)["wifi station ip"] = WifiStation.getIP().toString();
	(*map)["wifi station mac"] = WifiStation.getMAC();
	(*map)["wifi station brodcast"] = WifiStation.getNetworkBroadcast().toString();
	(*map)["wifi station gateway"] = WifiStation.getNetworkGateway().toString();
	(*map)["wifi station mask"] = WifiStation.getNetworkMask().toString();
	if(showPassword) {
		(*map)["wifi station password"] = WifiStation.getPassword();
	}
	(*map)["wifi station Rssi"] = WifiStation.getRssi();
	(*map)["wifi station SSID"] = WifiStation.getSSID();

	(*map)["wifi AP enabled"] = WifiAccessPoint.isEnabled() ? "true" : "false";
	(*map)["wifi AP ip"] = WifiAccessPoint.getIP().toString();
	(*map)["wifi AP mac"] = WifiAccessPoint.getMAC();
	(*map)["wifi AP broadcast"] = WifiAccessPoint.getNetworkBroadcast().toString();
	(*map)["wifi AP gateway"] = WifiAccessPoint.getNetworkGateway().toString();
	(*map)["wifi AP mask"] = WifiAccessPoint.getNetworkMask().toString();
	if(showPassword) {
		(*map)["wifi AP password"] = WifiAccessPoint.getPassword();
	}
	(*map)["wifi AP SSID"] = WifiAccessPoint.getSSID();


	return map;
}

