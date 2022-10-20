#pragma once
#include "pl_network.h"

//==============================================================================

namespace PL {

//==============================================================================

/// @brief DNS service instance additional information item
struct MdnsServiceInstanceAdditionalInfoItem {
  /// @brief Name
  std::string name;
  /// @brief Value
  std::string value;
};

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
  std::vector<MdnsServiceInstanceAdditionalInfoItem> additionalInfo;
};

//==============================================================================

}