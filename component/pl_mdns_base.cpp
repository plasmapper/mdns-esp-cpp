#include "pl_mdns_base.h"
#include "mdns.h"
#include "esp_check.h"

//==============================================================================

static const char* TAG = "pl_mdns";

//==============================================================================

namespace PL {

//==============================================================================

Mutex Mdns::mutex;
int Mdns::count = 0;

//==============================================================================

esp_err_t Mdns::Init() {
  LockGuard lg(mutex);
  if (count == 0)
    ESP_RETURN_ON_ERROR(mdns_init(), TAG, "init failed");
  count++;
  return ESP_OK;
}

//==============================================================================

void Mdns::Free() {
  LockGuard lg(mutex);
  if (count == 0)
    return;
  if (--count == 0)
    mdns_free();
}

//==============================================================================

}
