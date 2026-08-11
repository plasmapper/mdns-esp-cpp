cmake_minimum_required(VERSION 3.22)

idf_component_register(SRCS "pl_mdns_base.cpp" "pl_mdns_client.cpp" "pl_mdns_server.cpp" INCLUDE_DIRS "include" REQUIRES "mdns" "pl_common" "pl_network")