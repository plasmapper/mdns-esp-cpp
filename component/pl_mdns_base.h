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
  struct State {
    Mutex mutex;
    int count = 0;
  };
  // A function-local static rather than class-static data members, so construction happens on
  // first use instead of depending on static initialization order across translation units.
  static State& GetState();

  // Acquires the shared mDNS resource, calling mdns_init() only if it is not already held
  static esp_err_t Init();
  // Releases the shared mDNS resource, calling mdns_free() only once nothing holds it anymore
  static void Free();
};

//==============================================================================

}
