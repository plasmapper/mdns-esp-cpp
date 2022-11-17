#pragma once
#include "pl_common.h"
#include "pl_network.h"
#include "pl_mdns_types.h"

//==============================================================================

namespace PL {

//==============================================================================

/// @brief mDNS client class
class MdnsClient : public Lockable{
public:
  /// @brief Default read operation timeout in FreeRTOS ticks
  static const TickType_t defaultReadTimeout = 300 / portTICK_PERIOD_MS;

  /// @brief Create an mDNS client
  MdnsClient() {}

  esp_err_t Lock (TickType_t timeout = portMAX_DELAY) override;
  esp_err_t Unlock() override;

  /// @brief Discover services using a DNS-SD query
  /// @param type service type (_http, _ftp, etc)
  /// @param protocol service protocol (_tcp, _udp)
  /// @param maxNumberOfInstances maximum number of service instances to discover
  /// @param instanceName if not empty - only the services that contain this string in the name are returned 
  /// @param timeout query timeout in FreeRTOS ticks
  /// @param serviceInstancesInfo discovered services
  /// @return error code
  esp_err_t DnsSdQuery (const std::string& type, const std::string& protocol, size_t maxNumberOfInstances, const std::string& instanceName,
                               std::vector<MdnsServiceInstanceInfo>& serviceInstancesInfo);

  /// @brief Get the read operation timeout 
  /// @return timeout in FreeRTOS ticks
  TickType_t GetReadTimeout();

  /// @brief Set the read operation timeout 
  /// @param timeout timeout in FreeRTOS ticks
  /// @return error code
  esp_err_t SetReadTimeout (TickType_t timeout);

private:
  Mutex mutex;
  TickType_t readTimeout = NetworkStream::defaultReadTimeout;
};


//==============================================================================

}