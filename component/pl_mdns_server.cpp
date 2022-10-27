#include "pl_mdns_server.h"
#include "mdns.h"

//==============================================================================

namespace PL {

//==============================================================================

const std::string MdnsServer::defaultName = "mDNS Server";

//==============================================================================

MdnsServer::MdnsServer (const std::string& hostname) : hostname (hostname), serverEventHandler (this, [](EventHandler<Server>*){}) {
  SetName (defaultName);
}

//==============================================================================

MdnsServer::~MdnsServer() {
  for (auto& service : services) {
    if (auto serverLocked = service.server.lock()) {
      serverLocked->enabledEvent.RemoveHandler (serverEventHandler);
      serverLocked->disabledEvent.RemoveHandler (serverEventHandler);
    }
  }
}

//==============================================================================

esp_err_t MdnsServer::Lock (TickType_t timeout) {
  return mutex.Lock (timeout);
}

//==============================================================================

esp_err_t MdnsServer::Unlock() {
  return mutex.Unlock();
}

//==============================================================================

esp_err_t MdnsServer::Enable() {
  LockGuard lg (mutex);
  if (enabled)
    return ESP_OK;

  PL_RETURN_ON_ERROR (mdns_init());
  PL_RETURN_ON_ERROR (mdns_hostname_set (hostname.c_str()));

  enabled = true;
  enabledEvent.Generate();

  for (auto& service : services) {
    if (auto serverLocked = service.server.lock()) {
      if (serverLocked->IsEnabled()) {
        std::unique_ptr<mdns_txt_item_t[]> txtItems;
        size_t numberOfTxtItems = service.additionalInfo.size();
        if (numberOfTxtItems) {
          txtItems = std::unique_ptr<mdns_txt_item_t[]> (new mdns_txt_item_t[numberOfTxtItems]);
          size_t index = 0;
          for (auto& additionalInfoItem : service.additionalInfo) {
            txtItems[index].key = additionalInfoItem.first.c_str();
            txtItems[index++].value = additionalInfoItem.second.c_str();
          }            
        }          

        PL_RETURN_ON_ERROR (mdns_service_add (serverLocked->GetName().c_str(), service.type.c_str(), service.protocol.c_str(), serverLocked->GetPort(), txtItems.get(), numberOfTxtItems));
      }
    }
  }

  return ESP_OK;
}

//==============================================================================

esp_err_t MdnsServer::Disable() {
  LockGuard lg (mutex);
  if (!enabled)
    return ESP_OK;

  mdns_free();
  enabled = false;
  disabledEvent.Generate();
  return ESP_OK;
}

//==============================================================================

esp_err_t MdnsServer::AddService (std::shared_ptr<NetworkServer> server, const std::string& type, const std::string& protocol,
                                  const std::map<std::string, std::string>& additionalInfo) {
  LockGuard lg (mutex);
  services.push_back ({server, type, protocol, additionalInfo});
  server->enabledEvent.AddHandler (serverEventHandler);
  server->disabledEvent.AddHandler (serverEventHandler);
  if (enabled) {
    Disable();
    return Enable();
  }
  return ESP_OK;
}

//==============================================================================

esp_err_t MdnsServer::RemoveService (std::shared_ptr<NetworkServer> server) {
  for (auto service = services.begin(); service != services.end();) {
    if (auto serverLocked = service->server.lock()) {
      if (serverLocked == server)
        services.erase (service);
      else
        service++;
    }
    else
      service++;
  }
  if (enabled) {
    Disable();
    return Enable();
  }
  return ESP_OK;
}

//==============================================================================

bool MdnsServer::IsEnabled() {
  LockGuard lg (mutex);
  return enabled;
}

//==============================================================================

uint16_t MdnsServer::GetPort() {
  return defaultPort;
}

//==============================================================================

esp_err_t MdnsServer::SetPort (uint16_t port) {
  return ESP_ERR_NOT_SUPPORTED;
}

//==============================================================================

size_t MdnsServer::GetMaxNumberOfClients() {
  return defaultMaxNumberOfClients;
}

//==============================================================================

esp_err_t MdnsServer::SetMaxNumberOfClients (size_t maxNumberOfClients) {
  return ESP_ERR_NOT_SUPPORTED;
}

//==============================================================================

std::string MdnsServer::GetHostname() {
  LockGuard lg (mutex);
  return hostname;
}

//==============================================================================

esp_err_t MdnsServer::SetHostname (const std::string& hostname) {
  LockGuard lg (mutex);
  this->hostname = hostname;
  if (enabled) {
    Disable();
    Enable();
  }
  return ESP_OK;
}

//==============================================================================

void MdnsServer::HandleEvent (Server& server) {
  LockGuard lg (mutex);
  if (enabled) {
    Disable();
    Enable();
  }
}

//==============================================================================

}