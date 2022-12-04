#include "pl_mdns_client.h"
#include "mdns.h"
#include "esp_check.h"

//==============================================================================

static const char* TAG = "pl_mdns_client";

//==============================================================================

namespace PL {

//==============================================================================

esp_err_t MdnsClient::Lock (TickType_t timeout) {
  esp_err_t error = mutex.Lock (timeout);
  if (error == ESP_OK)
    return ESP_OK;
  if (error == ESP_ERR_TIMEOUT && timeout == 0)
    return ESP_ERR_TIMEOUT;
  ESP_RETURN_ON_ERROR (error, TAG, "mutex lock failed");
  return ESP_OK;
}

//==============================================================================

esp_err_t MdnsClient::Unlock() {
  ESP_RETURN_ON_ERROR (mutex.Unlock(), TAG, "mutex unlock failed");
  return ESP_OK;
}

//==============================================================================

esp_err_t MdnsClient::DnsSdQuery (const std::string& type, const std::string& protocol, size_t maxNumberOfInstances, const std::string& instanceName,
                                  std::vector<MdnsServiceInstanceInfo>& serviceInstancesInfo) {
  LockGuard lg (*this);
  mdns_result_t* results = NULL;
  serviceInstancesInfo.clear();

  ESP_RETURN_ON_ERROR (mdns_init(), TAG, "init failed");
  ESP_RETURN_ON_ERROR (mdns_query (NULL, type.c_str(), protocol.c_str(), MDNS_TYPE_PTR, readTimeout * portTICK_PERIOD_MS, maxNumberOfInstances, &results), TAG, "query failed");
  if (results) {
    for (mdns_result_t* r = results; r; r = r->next) {
      MdnsServiceInstanceInfo info;
      info.instanceName = r->instance_name;

      if (info.instanceName.empty() || info.instanceName.find (instanceName) != std::string::npos) {
        info.hostname = r->hostname;

        if (r->addr) {
          if (r->addr->addr.type == IPADDR_TYPE_V4)
            info.networkEndpoint = NetworkEndpoint (IpV4Address (r->addr->addr.u_addr.ip4.addr), r->port);
          if (r->addr->addr.type == IPADDR_TYPE_V6)
            info.networkEndpoint = NetworkEndpoint (IpV6Address (*(IpV6Address*)&r->addr->addr.u_addr.ip6.addr), r->port);
        }

        for (size_t i = 0; i < r->txt_count; i++)
          info.additionalInfo[r->txt[i].key] = r->txt[i].value;

        serviceInstancesInfo.push_back (info);        
      }
    }      
    mdns_query_results_free(results);
  }

  return ESP_OK;  
}

//==============================================================================

TickType_t MdnsClient::GetReadTimeout() {
  LockGuard lg (*this);
  return readTimeout;
}

//==============================================================================

esp_err_t MdnsClient::SetReadTimeout (TickType_t timeout) {
  LockGuard lg (*this);
  this->readTimeout = timeout;
  return ESP_OK;
}

//==============================================================================

};