// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Document/JsonDocument.hpp>
#ifdef ARDUINO_ARCH_ESP32
#include <esp_heap_caps.h>
#endif

namespace ARDUINOJSON_NAMESPACE {

#ifdef ARDUINO_ARCH_ESP32
template <size_t desiredCapacity>
class StaticJsonDocument : public JsonDocument {
  static const size_t _capacity =
      AddPadding<Max<1, desiredCapacity>::value>::value;

 public:
  StaticJsonDocument() : JsonDocument(_buffer = (char*)heap_caps_malloc(_capacity, MALLOC_CAP_DEFAULT), _capacity) {}

  StaticJsonDocument(const StaticJsonDocument& src)
      : JsonDocument(_buffer = heap_caps_malloc(_capacity, MALLOC_CAP_DEFAULT), _capacity) {
    set(src);
  }

  template <typename T>
  StaticJsonDocument(
      const T& src,
      typename enable_if<is_convertible<T, JsonVariantConst>::value>::type* = 0)
      : JsonDocument(_buffer = (char*)heap_caps_malloc(_capacity, MALLOC_CAP_DEFAULT), _capacity) {
    set(src);
  }

  // disambiguate
  StaticJsonDocument(JsonVariant src) : JsonDocument(_buffer = (char*)heap_caps_malloc(_capacity, MALLOC_CAP_DEFAULT), _capacity) {
    set(src);
  }

  ~StaticJsonDocument() {
    heap_caps_free((void*)_buffer);
  }

  StaticJsonDocument& operator=(const StaticJsonDocument& src) {
    set(src);
    return *this;
  }

  template <typename T>
  StaticJsonDocument& operator=(const T& src) {
    set(src);
    return *this;
  }

  // Reclaims the memory leaked when removing and replacing values.
  // https://arduinojson.org/v6/api/jsondocument/garbagecollect/
  void garbageCollect() {
    StaticJsonDocument tmp(*this);
    set(tmp);
  }

 private:
  char* _buffer;
};
#else
// A JsonDocument with a memory pool on the stack.
template <size_t desiredCapacity>
class StaticJsonDocument : public JsonDocument {
  static const size_t _capacity =
      AddPadding<Max<1, desiredCapacity>::value>::value;

 public:
  StaticJsonDocument() : JsonDocument(_buffer, _capacity) {}

  StaticJsonDocument(const StaticJsonDocument& src)
      : JsonDocument(_buffer, _capacity) {
    set(src);
  }

  template <typename T>
  StaticJsonDocument(
      const T& src,
      typename enable_if<is_convertible<T, JsonVariantConst>::value>::type* = 0)
      : JsonDocument(_buffer, _capacity) {
    set(src);
  }

  // disambiguate
  StaticJsonDocument(JsonVariant src) : JsonDocument(_buffer, _capacity) {
    set(src);
  }

  StaticJsonDocument& operator=(const StaticJsonDocument& src) {
    set(src);
    return *this;
  }

  template <typename T>
  StaticJsonDocument& operator=(const T& src) {
    set(src);
    return *this;
  }

  // Reclaims the memory leaked when removing and replacing values.
  // https://arduinojson.org/v6/api/jsondocument/garbagecollect/
  void garbageCollect() {
    StaticJsonDocument tmp(*this);
    set(tmp);
  }

 private:
  char _buffer[_capacity];
};
#endif

}  // namespace ARDUINOJSON_NAMESPACE
