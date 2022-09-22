# Sleep Timeout Usermod

This is intended for battery operated devices.

Puts the device to sleep. For the ESP32 you could set a pin for waking it up, for the ESP8266 you need to use the *RST* pin.

Note that any WLED functionality will not be available after the device goes to sleep, the idea is to lower the consumption to the minimum.

## Setup

Add #define `USERMOD_SLEEP_TIMEOUT` to `my_config.h`