#include "pl_mdns.h"
#include "pl_network.h"

//==============================================================================

class TestTcpServer : public PL::TcpServer {
public:
  using PL::TcpServer::TcpServer;

protected:
  esp_err_t HandleRequest(PL::NetworkStream& clientStream) override;
};

//==============================================================================

void TestMdns();
