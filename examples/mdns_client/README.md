# mDNS Client Example

1. Internal ESP Wi-Fi station interface is created and initialized.
2. Wi-Fi station SSID and password are set from project configuration (can be changed in `Example` menu of [Project Configuration](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html)).
3. DNS-SD query is used to discover HTTP services.