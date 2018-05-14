#include "InfoProvider.h"

#include <SmingCore/SmingCore.h>
#include <Wiring/WHashMap.h>

HashMap<String, String>* InfoProvider::getInfo(bool showPassword)  {
	auto map = new HashMap<String, String>;
	(*map)["sdk"] = system_get_sdk_version();
	(*map)["heap"] = String(system_get_free_heap_size(), 10);
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

