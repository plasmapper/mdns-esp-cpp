#include "mdns.h"
#include "unity.h"

//==============================================================================

const uint16_t port1 = 5678;
const uint16_t port2 = 5679;
const std::string hostname = "test-hostname-1";
const std::string newHostname = "test-hostname-2";
const std::string serviceType = "_srv";
const std::string serviceProtocol = "_tcp";
const std::string serviceName1 = "Test Service 1";
const std::string serviceName2 = "Test Service 2";
const std::map<std::string, std::string> additionalInfo = { {"A", "B"}, {"C", "D"} };
const TickType_t clientReadTimeout = 1000 / portTICK_PERIOD_MS;

//==============================================================================

void TestMdns() {
  auto tcpServer1 = std::make_shared<TestTcpServer>(port1);
  auto tcpServer2 = std::make_shared<TestTcpServer>(port2);
  PL::MdnsServer mdnsServer(hostname);
  PL::MdnsClient mdnsClient;

  // MdnsServer state before enabling
  TEST_ASSERT_EQUAL_STRING(hostname.c_str(), mdnsServer.GetHostname().c_str());
  TEST_ASSERT(!mdnsServer.IsEnabled());
  TEST_ASSERT_EQUAL(PL::MdnsServer::defaultPort, mdnsServer.GetPort());
  TEST_ASSERT_EQUAL(ESP_ERR_NOT_SUPPORTED, mdnsServer.SetPort(1234));
  TEST_ASSERT_EQUAL(PL::MdnsServer::defaultMaxNumberOfClients, mdnsServer.GetMaxNumberOfClients());
  TEST_ASSERT_EQUAL(ESP_ERR_NOT_SUPPORTED, mdnsServer.SetMaxNumberOfClients(5));

  // MdnsClient read timeout
  TEST_ASSERT_EQUAL(PL::MdnsClient::defaultReadTimeout, mdnsClient.GetReadTimeout());
  TEST_ASSERT(mdnsClient.SetReadTimeout(clientReadTimeout) == ESP_OK);
  TEST_ASSERT_EQUAL(clientReadTimeout, mdnsClient.GetReadTimeout());

  // Registering and enabling services
  TEST_ASSERT(tcpServer1->Enable() == ESP_OK);
  TEST_ASSERT(tcpServer2->Enable() == ESP_OK);
  TEST_ASSERT(mdnsServer.AddService(tcpServer1, serviceName1, serviceType, serviceProtocol, additionalInfo) == ESP_OK);
  TEST_ASSERT(mdnsServer.AddService(tcpServer2, serviceName2, serviceType, serviceProtocol) == ESP_OK);
  TEST_ASSERT(mdnsServer.Enable() == ESP_OK);
  TEST_ASSERT(mdnsServer.IsEnabled());

  // Toggling a registered server's enabled state should restart mDNS and leave it enabled
  TEST_ASSERT(tcpServer1->Disable() == ESP_OK);
  TEST_ASSERT(mdnsServer.IsEnabled());
  TEST_ASSERT(tcpServer1->Enable() == ESP_OK);
  TEST_ASSERT(mdnsServer.IsEnabled());

  // Removing a service should not affect the mDNS server's own enabled state
  TEST_ASSERT(mdnsServer.RemoveService(tcpServer1) == ESP_OK);
  TEST_ASSERT(mdnsServer.IsEnabled());

  // Changing the hostname should restart mDNS, leave it enabled and update the getter
  TEST_ASSERT(mdnsServer.SetHostname(newHostname) == ESP_OK);
  TEST_ASSERT_EQUAL_STRING(newHostname.c_str(), mdnsServer.GetHostname().c_str());
  TEST_ASSERT(mdnsServer.IsEnabled());

  // Disabling
  TEST_ASSERT(mdnsServer.Disable() == ESP_OK);
  TEST_ASSERT(!mdnsServer.IsEnabled());

  // Test client (response is not checked since there seems to be no guarantee that the device discovers itself) 
  std::vector<PL::MdnsServiceInstanceInfo> serviceInstancesInfo;
  TEST_ASSERT(mdnsClient.DnsSdQuery(serviceType, serviceProtocol, 10, "", serviceInstancesInfo) == ESP_OK);
}

//==============================================================================

TestTcpServer::~TestTcpServer() {
  StopTask();
}

//==============================================================================

esp_err_t TestTcpServer::HandleRequest(PL::NetworkStream& clientStream) {
  return ESP_OK;
}
