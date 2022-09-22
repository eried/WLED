#pragma once

#include "wled.h"

#define USERMOD_SLEEP_TIMEOUT_MINIMUM_MS 10000

#ifndef USERMOD_SLEEP_TIMEOUT_PIN
#ifdef ARDUINO_ARCH_ESP32
#define USERMOD_SLEEP_TIMEOUT_PIN 33
#else // ESP8266 boards
#define USERMOD_SLEEP_TIMEOUT_PIN 0
#endif
#endif

#ifndef USERMOD_SLEEP_TIMEOUT_MS
#define USERMOD_SLEEP_TIMEOUT_MS 1000 * 60 * 30 // 30 mins
#endif

// class name. Use something descriptive and leave the ": public Usermod" part :)
class SleepTimeout : public Usermod
{
private:
  // Private class members. You can declare variables and functions only accessible to your usermod here

#ifdef WLED_DEBUG
  unsigned long lastTime = 0;
#endif
  int8_t wakeupPin = USERMOD_SLEEP_TIMEOUT_PIN;
  unsigned int sleepAfterMs = USERMOD_SLEEP_TIMEOUT_MS;

public:
  void setup()
  {
    // Serial.println("Hello from my usermod!");
  }

  void connected()
  {
    // Serial.println("Connected to WiFi!");
  }

  void loop()
  {
#ifdef WLED_DEBUG
    if (millis() - lastTime > 1000)
    {
      DEBUG_PRINTLN(F("usermod_sleep_timeout 1 second elapsed"));
      lastTime = millis();
    }
#endif

    if (millis() > sleepAfterMs)
    {
      DEBUG_PRINTLN(F("GOING TO SLEEP"));
      strip.setBrightness(0);
      colorUpdated(CALL_MODE_DIRECT_CHANGE);
      bri = 0;
#ifdef ARDUINO_ARCH_ESP32
      esp_sleep_enable_ext0_wakeup((gpio_num_t)wakeupPin, 0); // 1 = High, 0 = Low
      esp_deep_sleep_start();
#else
      ESP.deepSleep(0);
#endif
    }
  }

  void addToJsonState(JsonObject &root)
  {
  }

  void readFromJsonState(JsonObject &root)
  {
  }

  void addToConfig(JsonObject &root)
  {
    JsonObject top = root.createNestedObject("Sleep-timeout"); // usermodname
#ifdef ARDUINO_ARCH_ESP32
    top["wake-up-pin"] = wakeupPin; // usermodparam
#endif
    top["sleep-after-ms"] = sleepAfterMs > USERMOD_SLEEP_TIMEOUT_MINIMUM_MS ? USERMOD_SLEEP_TIMEOUT_MINIMUM_MS : sleepAfterMs;
  }

  bool readFromConfig(JsonObject &root)
  {
    JsonObject top = root["Sleep-timeout"];

    bool configComplete = !top.isNull();
#ifdef ARDUINO_ARCH_ESP32
    configComplete &= getJsonValue(top["wake-up-pin"], wakeupPin, USERMOD_SLEEP_TIMEOUT_PIN);
#endif
    configComplete &= getJsonValue(top["sleep-after-ms"], sleepAfterMs, USERMOD_SLEEP_TIMEOUT_MS);
    return configComplete;
  }

  void handleOverlayDraw()
  {
  }

  uint16_t getId()
  {
    return USERMOD_ID_SLEEP_TIMEOUT;
  }
};