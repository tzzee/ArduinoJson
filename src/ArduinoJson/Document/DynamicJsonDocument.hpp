// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Document/BasicJsonDocument.hpp>

#include <stdlib.h>  // malloc, free
#ifdef ARDUINO_ARCH_ESP32
#include <esp_heap_caps.h>
#endif

namespace ARDUINOJSON_NAMESPACE {

// The allocator of DynamicJsonDocument.
struct DefaultAllocator {
  void* allocate(size_t size) {
#ifdef ARDUINO_ARCH_ESP32
    return heap_caps_malloc(size, MALLOC_CAP_DEFAULT);
#else
    return malloc(size);
#endif
  }

  void deallocate(void* ptr) {
#ifdef ARDUINO_ARCH_ESP32
    heap_caps_free(ptr);
#else
    free(ptr);
#endif
  }

  void* reallocate(void* ptr, size_t new_size) {
#ifdef ARDUINO_ARCH_ESP32
    return heap_caps_realloc(ptr, new_size, MALLOC_CAP_DEFAULT);
#else
    return realloc(ptr, new_size);
#endif
  }
};

// A JsonDocument with a memory pool in the heap.
// https://arduinojson.org/v6/api/dynamicjsondocument/
typedef BasicJsonDocument<DefaultAllocator> DynamicJsonDocument;

}  // namespace ARDUINOJSON_NAMESPACE
