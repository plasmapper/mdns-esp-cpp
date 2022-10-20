mDNS Component
==============

.. |COMPONENT| replace:: mdns

.. include:: ../../../installation.rst

.. include:: ../../../sdkconfig_network.rst

Features
--------

1. :cpp:class:`PL::MdnsClient` - an mDNS client class. Static function :cpp:func:`PL::MdnsClient::DnsSdQuery` discover services using a DNS-SD query.
2. :cpp:class:`PL::MdnsServer` - an mDNS server class. :cpp:func:`PL::MdnsServer::AddService` adds a :cpp:class:`PL::NetworkServer` to the mDNS server as a service. 

Examples
--------
| `mDNS client <https://github.com/plasmapper/mdns-esp-cpp/tree/main/examples/mdns_client>`_
| `mDNS server <https://github.com/plasmapper/mdns-esp-cpp/tree/main/examples/mdns_server>`_
  
API reference
-------------

.. toctree::
  
  api/types      
  api/mdns_client
  api/mdns_server