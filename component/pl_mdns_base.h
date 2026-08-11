#pragma once
#include "pl_common.h"

//==============================================================================

namespace PL {

//==============================================================================

/// @brief Shared mDNS resource acquisition and release, used internally by MdnsServer and MdnsClient
class Mdns {
  friend class MdnsServer;
  friend class MdnsClient;

private:
  static Mutex mutex;
  static int count;

  // Acquires the shared mDNS resource, calling mdns_init() only if it is not already held
  static esp_err_t Init();
  // Releases the shared mDNS resource, calling mdns_free() only once nothing holds it anymore
  static void Free();
};

//==============================================================================

}
