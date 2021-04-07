#include "StatusProvider.h"

#include <SmingCore.h>
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

Status StatusProvider::getStatus(bool showPassword)  {
	Status status;

	status.smingVersion = SMING_VERSION;
	status.sdk = system_get_sdk_version();
	status.time = SystemClock.getSystemTimeString();
	status.systemRestartReason = restartReasonString();

	char buf[128] = {0};
	sprintf(buf, "%d.%d.%d", LWIP_VERSION_MAJOR, LWIP_VERSION_MINOR, LWIP_VERSION_REVISION);
	status.lwIpVersion = String(buf);

	status.cpuFrequency = system_get_cpu_freq();
	status.chipId = system_get_chip_id();
	status.flashId = spi_flash_get_id();

	auto heap_free = system_get_free_heap_size();
	status.heapCurrentFreeMemory = heap_free;

#ifdef ENABLE_MALLOC_COUNT
	auto heap_current = MallocCount::getCurrent();
	auto heap_size = heap_current+heap_free;
	float heap_perc = heap_current;
	heap_perc = heap_perc/heap_size*100;
	status.heapUsagePercentage = heap_perc;
	status.heapCurrentMemoryAllocated = heap_current;
	status.heapPeakMemoryAllocated = MallocCount::getPeak();
	status.heapNumberOfAllocationsCalled = MallocCount::getAllocCount();
	status.heapCumulativeMemoryAllocated = MallocCount::getTotal();
#endif

	status.wifiStationEnabled = WifiStation.isEnabled();
	status.wifiStationChannel = WifiStation.getChannel();
	status.wifiStationIsConnected = WifiStation.isConnected();
	status.wifiStationIsConnectionFailed = WifiStation.isConnectionFailed();
	status.wifiStationDhcpEnaled = WifiStation.isEnabledDHCP();
	status.wifiStationConnectionStatus = WifiStation.getConnectionStatusName();
	status.wifiStationHostname = WifiStation.getHostname();
	status.wifiStationIp = WifiStation.getIP();
	status.wifiStationMac = WifiStation.getMAC();
	status.wifiStationBrodcast = WifiStation.getNetworkBroadcast();
	status.wifiStationGateway = WifiStation.getNetworkGateway();
	status.wifiStationMask = WifiStation.getNetworkMask();
	if(showPassword) {
		status.wifiStationPassword = WifiStation.getPassword();
	}
	status.wifiStationRssi = WifiStation.getRssi();
	status.wifiStationSsid = WifiStation.getSSID();


	status.wifiApEnabled = WifiAccessPoint.isEnabled() ? "true" : "false";
	status.wifiApIp = WifiAccessPoint.getIP();
	status.wifiApMac = WifiAccessPoint.getMAC();
	status.wifiApBroadcast = WifiAccessPoint.getNetworkBroadcast();
	status.wifiApGateway = WifiAccessPoint.getNetworkGateway();
	status.wifiApMask = WifiAccessPoint.getNetworkMask();
	if(showPassword) {
		status.wifiApPassword = WifiAccessPoint.getPassword();
	}
	status.wifiApSsid = WifiAccessPoint.getSSID();

	return status;
}

