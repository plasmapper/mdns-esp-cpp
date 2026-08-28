#include "pl_mdns_base.h"
#include "mdns.h"
#include "esp_check.h"

//==============================================================================

static const char* TAG = "pl_mdns";

//==============================================================================

namespace PL {

//==============================================================================

Mdns::State& Mdns::GetState() {
  static State state;
  return state;
}

//==============================================================================

esp_err_t Mdns::Init() {
  State& state = GetState();
  LockGuard lg(state.mutex);
  if (state.count == 0)
    ESP_RETURN_ON_ERROR(mdns_init(), TAG, "init failed");
  state.count++;
  return ESP_OK;
}

//==============================================================================

void Mdns::Free() {
  State& state = GetState();
  LockGuard lg(state.mutex);
  if (state.count == 0)
    return;
  if (--state.count == 0)
    mdns_free();
}

//==============================================================================

}
