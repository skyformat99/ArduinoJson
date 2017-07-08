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

  // TODO: remove hardcoded 8

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
    int bit = 256;
    for (int8_t index = 8; index >= 0; index--) {
      if (value / 10 < FloatTraits<T>::negativeBinaryPowerOfTen(index)) {
        value *= FloatTraits<T>::positiveBinaryPowerOfTen(index);
        powersOf10 = int16_t(powersOf10 - bit);
      }
      bit >>= 1;
    }
  }

  return powersOf10;
}
}
}
