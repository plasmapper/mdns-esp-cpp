#include "pl_network.h"
#include "pl_mdns.h"

//==============================================================================

PL::EspWiFiStation wifi;
const std::string wifiSsid = CONFIG_EXAMPLE_WIFI_SSID;
const std::string wifiPassword = CONFIG_EXAMPLE_WIFI_PASSWORD;

const std::string serviceType = "_http";
const std::string serviceProtocol = "_tcp";
const TickType_t timeout = 1000 / portTICK_RATE_MS;

//==============================================================================

extern "C" void app_main(void) {
  esp_event_loop_create_default();
  esp_netif_init();

  wifi.Initialize();
  wifi.SetSsid (wifiSsid);
  wifi.SetPassword (wifiPassword);
  wifi.Enable();

  while (!wifi.GetIpV4Address().u32)  
    vTaskDelay (1);
  
  std::vector<PL::MdnsServiceInstanceInfo> serviceInstancesInfo;
  ESP_ERROR_CHECK_WITHOUT_ABORT (PL::MdnsClient::DnsSdQuery (serviceType, serviceProtocol, 10, "", timeout, serviceInstancesInfo));

  printf ("Discovered %d %s.%s.local services.\n\n", serviceInstancesInfo.size(), serviceType.c_str(), serviceProtocol.c_str());

  for (auto& info : serviceInstancesInfo) {
    printf ("Instance name: %s\n", info.instanceName.c_str());
    printf ("Hostname: %s\n", info.hostname.c_str());
    printf ("Address: %s\n", info.networkEndpoint.address.ToString().c_str());
    printf ("Port: %d\n", info.networkEndpoint.port);
    if (info.additionalInfo.size()) {
      printf ("Additional information:\n");
      for (auto& additionalInfoItem : info.additionalInfo)
        printf ("  %s: %s\n", additionalInfoItem.name.c_str(), additionalInfoItem.value.c_str());
    }
    
    printf ("\n");
  }
}