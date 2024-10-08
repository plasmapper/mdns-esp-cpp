mDNS Component
==============

.. |COMPONENT| replace:: mdns

.. |ESP_IDF_VERSION| replace:: 5.3
  
.. |VERSION| replace:: 1.1.0

.. include:: ../../../installation.rst

.. include:: ../../../sdkconfig_network.rst

Features
--------

1. :cpp:class:`PL::MdnsClient` - an mDNS client class. :cpp:func:`PL::MdnsClient::DnsSdQuery` discovers services using a DNS-SD query.
2. :cpp:class:`PL::MdnsServer` - an mDNS server class. :cpp:func:`PL::MdnsServer::AddService` adds a :cpp:class:`PL::NetworkServer` to the mDNS server as a service. 

Thread safety
-------------

Class method thread safety is implemented by having the :cpp:class:`PL::Lockable` as a base class and creating the class object lock guard at the beginning of the methods.

Examples
--------
| `mDNS client <https://components.espressif.com/components/plasmapper/pl_mdns/versions/1.1.0/examples/mdns_client>`_
| `mDNS server <https://components.espressif.com/components/plasmapper/pl_mdns/versions/1.1.0/examples/mdns_server>`_
  
API reference
-------------

.. toctree::
  
  api/types      
  api/mdns_client
  api/mdns_server