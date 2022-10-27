#pragma once
#include "pl_network.h"
#include <map>

//==============================================================================

namespace PL {

//==============================================================================

/// @brief mDNS service instance information
struct MdnsServiceInstanceInfo {
  /// @brief Instance name
  std::string instanceName;
  /// @brief Hostname
  std::string hostname;
  /// @brief Network endpoint
  NetworkEndpoint networkEndpoint;
  /// @brief Additional information
  std::map<std::string, std::string> additionalInfo;
};

//==============================================================================

}