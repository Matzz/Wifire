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
    bool wifiStationDhcpEnabled = false;
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


template<>
void Codec<Status>::encode(JsonObject& json, const Status &cfg);
