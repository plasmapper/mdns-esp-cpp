#pragma once
#include "pl_common.h"
#include "pl_network.h"
#include "pl_mdns_types.h"

//==============================================================================

namespace PL {

//==============================================================================

/// @brief mDNS server class
class MdnsServer : public NetworkServer, public EventHandler<Server> {
public:
  /// @brief Default server name
  static const std::string defaultName;
  /// @brief Default port
  static const uint16_t defaultPort = 5353;
  /// @brief Default maximum number of server clients
  static const int defaultMaxNumberOfClients = 1;
  
  /// @brief Create an mDNS server
  /// @param hostname hostname
  MdnsServer (const std::string& hostname);
  ~MdnsServer();
  MdnsServer (const MdnsServer&) = delete;
  MdnsServer& operator= (const MdnsServer&) = delete;

  esp_err_t Lock (TickType_t timeout = portMAX_DELAY) override;
  esp_err_t Unlock() override;

  esp_err_t Enable() override;
  esp_err_t Disable() override;

  void HandleEvent (Server& server) override;

  /// @brief Add a network server to the mDNS server as a service
  /// @param server server
  /// @param type service type (_http, _ftp, etc)
  /// @param protocol service protocol (_tcp, _udp)
  /// @param additionalInfo additional information
  /// @return 
  esp_err_t AddService (std::shared_ptr<NetworkServer> server, const std::string& type, const std::string& protocol,
                        const std::map<std::string, std::string>& additionalInfo = {});

  // Remove the network server service from the mDNS server
  esp_err_t RemoveService (std::shared_ptr<NetworkServer> server);

  bool IsEnabled() override;

  uint16_t GetPort() override;
  esp_err_t SetPort (uint16_t port) override;

  size_t GetMaxNumberOfClients() override;
  esp_err_t SetMaxNumberOfClients (size_t maxNumberOfClients) override;

  /// @brief Get the hostname
  /// @return hostname
  std::string GetHostname();

  /// @brief Set the hostname
  /// @param hostname 
  /// @return error code
  esp_err_t SetHostname (const std::string& hostname);

private:
  Mutex mutex;
  bool enabled = false;
  std::string hostname;
  std::shared_ptr<EventHandler<Server>> serverEventHandler;

  struct Service {
    std::weak_ptr<NetworkServer> server;
    std::string type;
    std::string protocol;
    std::map<std::string, std::string> additionalInfo;
  };
  std::vector<Service> services;  
};

//==============================================================================

}