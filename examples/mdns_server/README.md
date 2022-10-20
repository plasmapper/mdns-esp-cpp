# mDNS Server Example

1. EchoServer class inherits TcpServer class and overrides HandleRequest method to implement echo functionality.
2. Internal ESP Wi-Fi station interface is created and initialized.
3. Wi-Fi station SSID and password are set from project configuration (can be changed in `Example` menu of [Project Configuration](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html)).
4. The echo server is added as a service to the mDNS server; 
5. The Wi-Fi station is enabled.
6. The echo server is enabled.
7. The mDNS server is enabled.
