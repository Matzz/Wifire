#include "Status.h"

template<>
void Codec<Status>::encode(JsonObject& json, const Status &cfg) {
    json["smingVersion"] = cfg.smingVersion;
    json["sdk"] = cfg.sdk;
    json["time"] = cfg.time;
    json["systemRestartReason"] = cfg.systemRestartReason;
    json["lwIpVersion"] = cfg.lwIpVersion;
    json["cpuFrequency"] = cfg.cpuFrequency;
    json["chipId"] = cfg.chipId;
    json["flashId"] = cfg.flashId;
    json["heapCurrentFreeMemory"] = cfg.heapCurrentFreeMemory;
    if(cfg.heapUsagePercentage>=0) {
        json["heapUsagePercentage"] = cfg.heapUsagePercentage;
        json["heapCurrentMemoryAllocated"] = cfg.heapCurrentMemoryAllocated;
        json["heapPeakMemoryAllocated"] = cfg.heapPeakMemoryAllocated;
        json["heapNumberOfAllocationsCalled"] = cfg.heapNumberOfAllocationsCalled;
        json["heapCumulativeMemoryAllocated"] = cfg.heapCumulativeMemoryAllocated;
    }
    json["wifiStationEnabled"] = cfg.wifiStationEnabled;
    json["wifiStationChannel"] = cfg.wifiStationChannel;
    json["wifiStationIsConnected"] = cfg.wifiStationIsConnected;
    json["wifiStationIsConnectionFailed"] = cfg.wifiStationIsConnectionFailed;
    json["wifiStationDhcpEnabled"] = cfg.wifiStationDhcpEnabled;
    json["wifiStationConnectionStatus"] = cfg.wifiStationConnectionStatus;
    json["wifiStationHostname"] = cfg.wifiStationHostname;
    json["wifiStationIp"] = CodecHelpers::ipToString(cfg.wifiStationIp);
    json["wifiStationMac"] = cfg.wifiStationMac;
    json["wifiStationBrodcast"] = CodecHelpers::ipToString(cfg.wifiStationBrodcast);
    json["wifiStationGateway"] = CodecHelpers::ipToString(cfg.wifiStationGateway);
    json["wifiStationMask"] = CodecHelpers::ipToString(cfg.wifiStationMask);;
    json["wifiStationPassword"] = cfg.wifiStationPassword;
    json["wifiStationRssi"] = cfg.wifiStationRssi;
    json["wifiStationSsid"] = cfg.wifiStationSsid;
    json["wifiApEnabled"] = cfg.wifiApEnabled;
    json["wifiApIp"] = CodecHelpers::ipToString(cfg.wifiApIp);
    json["wifiApMac"] = cfg.wifiApMac;
    json["wifiApBroadcast"] = CodecHelpers::ipToString(cfg.wifiApBroadcast);
    json["wifiApGateway"] = CodecHelpers::ipToString(cfg.wifiApGateway);
    json["wifiApMask"] = CodecHelpers::ipToString(cfg.wifiApMask);
    json["wifiApPassword"] = cfg.wifiApPassword;
    json["wifiApSsid"] = cfg.wifiApSsid;
}