#include "pl_mdns_server.h"
#include "pl_mdns_base.h"
#include "mdns.h"
#include "esp_check.h"

//==============================================================================

static const char* TAG = "pl_mdns_server";

//==============================================================================

namespace PL {

//==============================================================================

const std::string MdnsServer::defaultName = "mDNS Server";

//==============================================================================

MdnsServer::MdnsServer(const std::string& hostname) : hostname(hostname), serverEventHandler(this, [](EventHandler<Server>*){}) {
  SetName(defaultName);
}

//==============================================================================

MdnsServer::~MdnsServer() {
  LockGuard lg(*this);
  Disable();
  for (auto& service : services) {
    if (auto serverLocked = service.server.lock()) {
      serverLocked->enabledEvent.RemoveHandler(serverEventHandler);
      serverLocked->disabledEvent.RemoveHandler(serverEventHandler);
    }
  }
}

//==============================================================================

esp_err_t MdnsServer::Lock(TickType_t timeout) {
  esp_err_t error = mutex.Lock(timeout);
  if (error != ESP_OK && (error != ESP_ERR_TIMEOUT || timeout != 0))
    ESP_LOGE(TAG, "mutex lock failed");
  return error;
}

//==============================================================================

esp_err_t MdnsServer::Unlock() {
  ESP_RETURN_ON_ERROR(mutex.Unlock(), TAG, "mutex unlock failed");
  return ESP_OK;
}

//==============================================================================

esp_err_t MdnsServer::Enable() {
  LockGuard lg(*this);
  if (enabled)
    return ESP_OK;

  ESP_RETURN_ON_ERROR(Mdns::Init(), TAG, "init failed");

  esp_err_t error = mdns_hostname_set(hostname.c_str());
  if (error != ESP_OK)
    Mdns::Free();
  ESP_RETURN_ON_ERROR(error, TAG, "hostname set failed");

  for (auto& service : services) {
    if (auto serverLocked = service.server.lock()) {
      if (service.isEnabled) {
        std::unique_ptr<mdns_txt_item_t[]> txtItems;
        size_t numberOfTxtItems = service.additionalInfo.size();
        if (numberOfTxtItems) {
          txtItems = std::unique_ptr<mdns_txt_item_t[]>(new mdns_txt_item_t[numberOfTxtItems]);
          size_t index = 0;
          for (auto& additionalInfoItem : service.additionalInfo) {
            txtItems[index].key = additionalInfoItem.first.c_str();
            txtItems[index++].value = additionalInfoItem.second.c_str();
          }
        }

        error = mdns_service_add(service.name.c_str(), service.type.c_str(), service.protocol.c_str(), service.port, txtItems.get(), numberOfTxtItems);
        if (error != ESP_OK) {
          mdns_service_remove_all();
          Mdns::Free();
        }
        ESP_RETURN_ON_ERROR(error, TAG, "service add failed");
      }
    }
  }

  enabled = true;
  enabledEvent.Generate();
  return ESP_OK;
}

//==============================================================================

esp_err_t MdnsServer::Disable() {
  LockGuard lg(*this);
  if (!enabled)
    return ESP_OK;

  mdns_service_remove_all();
  Mdns::Free();
  enabled = false;
  disabledEvent.Generate();
  return ESP_OK;
}

//==============================================================================

void MdnsServer::HandleEvent(Server& server) {
  LockGuard lg(*this);
  for (auto& service : services) {
    if (auto serverLocked = service.server.lock()) {
      if (serverLocked.get() == &server) {
        service.isEnabled = serverLocked->IsEnabled();
        service.port = serverLocked->GetPort();
      }
    }
  }
  RestartIfEnabled();
}

//==============================================================================

esp_err_t MdnsServer::AddService(std::shared_ptr<NetworkServer> server, const std::string& name, const std::string& type, const std::string& protocol,
                                 const std::map<std::string, std::string>& additionalInfo) {
  bool serverIsEnabled = server->IsEnabled();
  uint16_t serverPort = server->GetPort();
  LockGuard lg(*this);
  services.push_back({server, serverIsEnabled, name, type, protocol, serverPort, additionalInfo});
  server->enabledEvent.AddHandler(serverEventHandler);
  server->disabledEvent.AddHandler(serverEventHandler);
  return RestartIfEnabled();
}

//==============================================================================

esp_err_t MdnsServer::RemoveService(std::shared_ptr<NetworkServer> server) {
  LockGuard lg(*this);
  for (auto service = services.begin(); service != services.end();) {
    if (auto serverLocked = service->server.lock()) {
      if (serverLocked == server) {
        serverLocked->enabledEvent.RemoveHandler(serverEventHandler);
        serverLocked->disabledEvent.RemoveHandler(serverEventHandler);
        service = services.erase(service);
      }
      else
        service++;
    }
    else
      service++;
  }
  return RestartIfEnabled();
}

//==============================================================================

bool MdnsServer::IsEnabled() {
  LockGuard lg(*this);
  return enabled;
}

//==============================================================================

uint16_t MdnsServer::GetPort() {
  return defaultPort;
}

//==============================================================================

esp_err_t MdnsServer::SetPort(uint16_t port) {
  return ESP_ERR_NOT_SUPPORTED;
}

//==============================================================================

size_t MdnsServer::GetMaxNumberOfClients() {
  return defaultMaxNumberOfClients;
}

//==============================================================================

esp_err_t MdnsServer::SetMaxNumberOfClients(size_t maxNumberOfClients) {
  return ESP_ERR_NOT_SUPPORTED;
}

//==============================================================================

std::string MdnsServer::GetHostname() {
  LockGuard lg(*this);
  return hostname;
}

//==============================================================================

esp_err_t MdnsServer::SetHostname(const std::string& hostname) {
  LockGuard lg(*this);
  this->hostname = hostname;
  return RestartIfEnabled();
}

//==============================================================================

esp_err_t MdnsServer::RestartIfEnabled() {
  if (!enabled)
    return ESP_OK;
  ESP_RETURN_ON_ERROR(Disable(), TAG, "disable failed");
  ESP_RETURN_ON_ERROR(Enable(), TAG, "enable failed");
  return ESP_OK;
}

//==============================================================================

}