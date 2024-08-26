#include "pl_network.h"
#include "pl_mdns.h"

//==============================================================================

class EchoServer : public PL::TcpServer {
public:
  static const std::string defaultName;
  static const uint16_t defaultPort = 7;

  EchoServer();
  esp_err_t HandleRequest(PL::NetworkStream& clientStream) override;
};

const std::string EchoServer::defaultName = "Echo Server";

//==============================================================================

PL::EspWiFiStation wifi;
const std::string wifiSsid = CONFIG_EXAMPLE_WIFI_SSID;
const std::string wifiPassword = CONFIG_EXAMPLE_WIFI_PASSWORD;

const std::string hostName = "ESP";
auto echoServer = std::make_shared<EchoServer>();
PL::MdnsServer mdnsServer(hostName);

//==============================================================================

extern "C" void app_main(void) {
  esp_event_loop_create_default();
  esp_netif_init();

  wifi.Initialize();
  wifi.SetSsid(wifiSsid);
  wifi.SetPassword(wifiPassword);

  mdnsServer.AddService(echoServer, EchoServer::defaultName, "_echo", "_tcp", { {"Test name", "Test value"} });

  wifi.EnableIpV4DhcpClient();
  wifi.Enable();
  echoServer->Enable();
  mdnsServer.Enable();

  while (1) {
    vTaskDelay(1);
  }
}

//==============================================================================

EchoServer::EchoServer() : TcpServer(defaultPort) {
  SetName(defaultName);
}

//==============================================================================

esp_err_t EchoServer::HandleRequest(PL::NetworkStream& clientStream) {
  uint8_t dataByte;
  while (clientStream.GetReadableSize()) {
    if (clientStream.Read(&dataByte, 1) == ESP_OK)
      clientStream.Write(&dataByte, 1);
  }  
  return ESP_OK; 
}