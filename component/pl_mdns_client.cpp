#include "pl_mdns_client.h"
#include "mdns.h"

//==============================================================================

namespace PL {

//==============================================================================

esp_err_t MdnsClient::DnsSdQuery (const std::string& type, const std::string& protocol, size_t maxNumberOfInstances, const std::string& instanceName, TickType_t timeout,
                                  std::vector<MdnsServiceInstanceInfo>& serviceInstancesInfo) {
  mdns_result_t* results = NULL;
  serviceInstancesInfo.clear();

  PL_RETURN_ON_ERROR (mdns_init());
  PL_RETURN_ON_ERROR (mdns_query (NULL, type.c_str(), protocol.c_str(), MDNS_TYPE_PTR, timeout * portTICK_RATE_MS, maxNumberOfInstances, &results));
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
          info.additionalInfo.push_back ({r->txt[i].key, r->txt[i].value});

        serviceInstancesInfo.push_back (info);        
      }
    }      
    mdns_query_results_free(results);
  }

  return ESP_OK;  
}

//==============================================================================

};