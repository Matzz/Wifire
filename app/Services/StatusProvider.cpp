#include "StatusProvider.h"

#include <SmingCore.h>
#include <WHashMap.h>
#include <esp_spi_flash.h>
#ifdef ENABLE_MALLOC_COUNT
#include <malloc_count.h>
#endif

String restartReasonString() {
	switch(system_get_rst_info()->reason) {
		case REASON_DEFAULT_RST: return "reason default restart";
		case REASON_WDT_RST: return "reason wdt restart";
		case REASON_EXCEPTION_RST: return "reason exception restart";
		case REASON_SOFT_WDT_RST: return "reason soft wdt restart";
		case REASON_SOFT_RESTART: return "reason soft restart";
		case REASON_DEEP_SLEEP_AWAKE: return "reason deephsleep awake";
		case REASON_EXT_SYS_RST: return "reason ext sys restart";
		default: return "unknon";
	}
}

HashMap<String, String>* StatusProvider::getStatus(bool showPassword)  {
	auto map = new HashMap<String, String>;
	HashMap<String, String> &mapRef = *map;

	mapRef["sming version"] = SMING_VERSION;
	mapRef["sdk"] = system_get_sdk_version();
	mapRef["time"] = SystemClock.getSystemTimeString();
	mapRef["system start reason"] = restartReasonString();

	char buf[128] = {0};
	sprintf(buf, "%d.%d.%d", LWIP_VERSION_MAJOR, LWIP_VERSION_MINOR, LWIP_VERSION_REVISION);
	mapRef["lwIP version"] = String(buf);

	auto heep_free = system_get_free_heap_size();
	mapRef["heep current free memory"] = String(heep_free, 10);

#ifdef ENABLE_MALLOC_COUNT
	auto heep_current = MallocCount::getCurrent();
	auto heep_size = heep_current+heep_free;
	float heep_perc = heep_current;
	heep_perc = heep_perc/heep_size*100;
	mapRef["heep current memory allocated"] = heep_current;
	mapRef["heep usage percentage"] = heep_perc;
	mapRef["heep peak memory allocated"] = MallocCount::getPeak();
	mapRef["heep number of allocations called"] = MallocCount::getAllocCount();
	mapRef["heep cumulative memory allocated"] = MallocCount::getTotal();
#endif

	mapRef["cpu frequency"] = String(system_get_cpu_freq(), 10);
	mapRef["chip id"] = String(system_get_chip_id(), 10);
	mapRef["flash id"] = String(spi_flash_get_id(), 10);

	mapRef["wifi station enabled"] = WifiStation.isEnabled() ? "true" : "false";
	mapRef["wifi station channel"] = String(WifiStation.getChannel(), 10);
	mapRef["wifi station is connected"] = WifiStation.isConnected() ? "true" : "false";
	mapRef["wifi station is connection failed"] = WifiStation.isConnectionFailed() ? "true" : "false";
	mapRef["wifi station DHCP enaled"] = WifiStation.isEnabledDHCP() ? "true" : "false";
	mapRef["wifi station connection status"] = WifiStation.getConnectionStatusName();
	String hostName = WifiStation.getHostname();
	mapRef["wifi station hostname"] = hostName != NULL ? hostName : "null";
	mapRef["wifi station ip"] = WifiStation.getIP().toString();
	mapRef["wifi station mac"] = WifiStation.getMAC();
	mapRef["wifi station brodcast"] = WifiStation.getNetworkBroadcast().toString();
	mapRef["wifi station gateway"] = WifiStation.getNetworkGateway().toString();
	mapRef["wifi station mask"] = WifiStation.getNetworkMask().toString();
	if(showPassword) {
		mapRef["wifi station password"] = WifiStation.getPassword();
	}
	mapRef["wifi station Rssi"] = WifiStation.getRssi();
	mapRef["wifi station SSID"] = WifiStation.getSSID();

	mapRef["wifi AP enabled"] = WifiAccessPoint.isEnabled() ? "true" : "false";
	mapRef["wifi AP ip"] = WifiAccessPoint.getIP().toString();
	mapRef["wifi AP mac"] = WifiAccessPoint.getMAC();
	mapRef["wifi AP broadcast"] = WifiAccessPoint.getNetworkBroadcast().toString();
	mapRef["wifi AP gateway"] = WifiAccessPoint.getNetworkGateway().toString();
	mapRef["wifi AP mask"] = WifiAccessPoint.getNetworkMask().toString();
	if(showPassword) {
		mapRef["wifi AP password"] = WifiAccessPoint.getPassword();
	}
	mapRef["wifi AP SSID"] = WifiAccessPoint.getSSID();

	return map;
}

