#pragma once
#include "pl_common.h"
#include "pl_network.h"
#include "pl_mdns_types.h"

//==============================================================================

namespace PL {

//==============================================================================

/// @brief mDNS server class
class MdnsServer : public NetworkServer {
public:
  /// @brief Default server name
  static constexpr const char* defaultName = "mDNS Server";
  /// @brief Default port
  static constexpr uint16_t defaultPort = 5353;
  /// @brief Default maximum number of server clients
  static constexpr int defaultMaxNumberOfClients = 1;
  
  /// @brief Creates an mDNS server
  /// @param hostname hostname
  MdnsServer(const std::string& hostname);
  ~MdnsServer();
  MdnsServer(const MdnsServer&) = delete;
  MdnsServer& operator=(const MdnsServer&) = delete;

  esp_err_t Lock(TickType_t timeout = portMAX_DELAY) override;
  esp_err_t Unlock() override;

  esp_err_t Enable() override;
  esp_err_t Disable() override;

  /// @brief Adds a network server to the mDNS server as a service
  /// @param server server
  /// @param name service name
  /// @param type service type (_http, _ftp, etc)
  /// @param protocol service protocol (_tcp, _udp)
  /// @param additionalInfo additional information
  /// @return error code
  esp_err_t AddService(std::shared_ptr<NetworkServer> server, const std::string& name, const std::string& type, const std::string& protocol,
                       const std::map<std::string, std::string>& additionalInfo = {});

  /// @brief Removes the network server service from the mDNS server
  /// @param server server
  /// @return error code
  esp_err_t RemoveService(std::shared_ptr<NetworkServer> server);

  bool IsEnabled() override;

  uint16_t GetPort() override;
  esp_err_t SetPort(uint16_t port) override;

  size_t GetMaxNumberOfClients() override;
  esp_err_t SetMaxNumberOfClients(size_t maxNumberOfClients) override;

  /// @brief Gets the hostname
  /// @return hostname
  std::string GetHostname();

  /// @brief Sets the hostname
  /// @param hostname 
  /// @return error code
  esp_err_t SetHostname(const std::string& hostname);

private:
  // A separate object rather than an alias to the MdnsServer itself, so that a handler call
  // already in flight keeps this object, not the MdnsServer, alive. The mutex is held for the
  // whole HandleEvent call, so Detach waits for an in-flight call to finish and any call after
  // it sees a null pointer and does nothing.
  class ServerEventHandler : public EventHandler<Server> {
  public:
    ServerEventHandler(MdnsServer& mdnsServer) : mdnsServer(&mdnsServer) {}

    void Detach();
    void HandleEvent(Server& server) override;

  private:
    Mutex mutex;
    MdnsServer* mdnsServer;
  };

  Mutex mutex;
  bool enabled = false;
  std::string hostname;

  struct Service {
    std::weak_ptr<NetworkServer> server;
    bool isEnabled;
    std::string name;
    std::string type;
    std::string protocol;
    uint16_t port;
    std::map<std::string, std::string> additionalInfo;
  };
  std::vector<Service> services;

  std::shared_ptr<ServerEventHandler> serverEventHandler;

  void HandleEvent(Server& server);
  esp_err_t RestartIfEnabled();
};

//==============================================================================

}