#pragma once

#include <SmingCore.h>
#include "Codec.h"

class Status {
public:
    String smingVersion = String::nullstr;
    String sdk = String::nullstr;
    String time = String::nullstr;
    String systemRestartReason = String::nullstr;
    String lwIpVersion = String::nullstr;

    unsigned char cpuFrequency = 0;
    unsigned int chipId = 0;
    unsigned int flashId = 0;

    long unsigned int heapCurrentFreeMemory = 0;

    //heapUsagePercentage == -1 indicate malloc_count is disabled (ENABLE_MALLOC_COUNT)
    float heapUsagePercentage = -1;
    long unsigned int heapCurrentMemoryAllocated = 0;
    long unsigned int heapPeakMemoryAllocated = 0;
    long unsigned int heapNumberOfAllocationsCalled = 0;
    long unsigned int heapCumulativeMemoryAllocated = 0;

    bool wifiStationEnabled = false;
    unsigned char wifiStationChannel = 0;
    bool wifiStationIsConnected = false;
    bool wifiStationIsConnectionFailed = false;
    bool wifiStationDhcpEnaled = false;
    String wifiStationConnectionStatus = String::nullstr;
    String wifiStationHostname = String::nullstr;
    IpAddress wifiStationIp = IPADDR_ANY;
    String wifiStationMac = String::nullstr;
    IpAddress wifiStationBrodcast = IPADDR_ANY;
    IpAddress wifiStationGateway = IPADDR_ANY;
    IpAddress wifiStationMask = IPADDR_ANY;
    String wifiStationPassword = String::nullstr;
    String wifiStationRssi = String::nullstr;
    String wifiStationSsid = String::nullstr;

    bool wifiApEnabled = false;
    IpAddress wifiApIp = String::nullstr;
    String wifiApMac = String::nullstr;
    IpAddress wifiApBroadcast = IPADDR_ANY;
    IpAddress wifiApGateway = IPADDR_ANY;
    IpAddress wifiApMask = IPADDR_ANY;
    IpAddress wifiApPassword = IPADDR_ANY;
    String wifiApSsid = String::nullstr;
};

template<> class Codec<Status> {
	public:
        static Codec<Status>& getInstance() {
            static Codec<Status> instance;
            return instance;
        }

	void encode(JsonObject& json, Status cfg) {
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
        json["wifiStationDhcpEnaled"] = cfg.wifiStationDhcpEnaled;
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

	Either<String, Status> decode(JsonObject& json) {
		Status cfg;
		cfg.smingVersion = F("decoder not implemented");
		return cfg;
	}
};