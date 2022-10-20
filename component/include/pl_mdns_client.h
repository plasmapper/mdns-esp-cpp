#pragma once
#include "pl_common.h"
#include "pl_network.h"
#include "pl_mdns_types.h"

//==============================================================================

namespace PL {

//==============================================================================

/// @brief mDNS client class
class MdnsClient {
public:
  /// @brief Discover services using a DNS-SD query
  /// @param type service type (_http, _ftp, etc)
  /// @param protocol service protocol (_tcp, _udp)
  /// @param maxNumberOfInstances maximum number of service instances to discover
  /// @param instanceName if not empty - only the services that contain this string in the name are returned 
  /// @param timeout query timeout in FreeRTOS ticks
  /// @param serviceInstancesInfo discovered services
  /// @return error code
  static esp_err_t DnsSdQuery (const std::string& type, const std::string& protocol, size_t maxNumberOfInstances, const std::string& instanceName, TickType_t timeout,
                               std::vector<MdnsServiceInstanceInfo>& serviceInstancesInfo); 
};


//==============================================================================

}