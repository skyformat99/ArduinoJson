// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"
#include "../TypeTraits/FloatTraits.hpp"

namespace ArduinoJson {
namespace Polyfills {
template <typename T>
int16_t normalize(T& value) {
  using namespace TypeTraits;
  int16_t powersOf10 = 0;

  if (value >= ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD) {
    int bit = 256;
    for (int8_t index = 8; index >= 0; index--) {
      if (value >= FloatTraits<T>::positiveBinaryPowerOfTen(index)) {
        value /= FloatTraits<T>::positiveBinaryPowerOfTen(index);
        powersOf10 = int16_t(powersOf10 + bit);
      }
      bit >>= 1;
    }
  }

  if (value > 0 && value <= ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD) {
#if ARDUINOJSON_DOUBLE_IS_64BITS
    if (value / 10 < 1e-256) {
      value *= 1e256;
      powersOf10 = int16_t(powersOf10 - 256);
    }
    if (value / 10 < 1e-128) {
      value *= 1e128;
      powersOf10 = int16_t(powersOf10 - 128);
    }
    if (value / 10 < 1e-64) {
      value *= 1e64;
      powersOf10 = int16_t(powersOf10 - 64);
    }
#endif
    if (value / 10 < 1e-32) {
      value *= 1e32;
      powersOf10 = int16_t(powersOf10 - 32);
    }
    if (value / 10 < 1e-16) {
      value *= 1e16;
      powersOf10 = int16_t(powersOf10 - 16);
    }
    if (value / 10 < 1e-8) {
      value *= 1e8;
      powersOf10 = int16_t(powersOf10 - 8);
    }
    if (value / 10 < 1e-4) {
      value *= 1e4;
      powersOf10 = int16_t(powersOf10 - 4);
    }
    if (value / 10 < 1e-2) {
      value *= 1e2;
      powersOf10 = int16_t(powersOf10 - 2);
    }
    if (value / 10 < 1e-1) {
      value *= 1e1;
      powersOf10 = int16_t(powersOf10 - 1);
    }
  }

  return powersOf10;
}
}
}
