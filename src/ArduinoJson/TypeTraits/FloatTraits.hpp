// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include <stdint.h>
#include <stdlib.h>  // for size_t
#include "../Configuration.hpp"
#include "../Polyfills/math.hpp"

namespace ArduinoJson {
namespace TypeTraits {

template <typename T, size_t = sizeof(T)>
struct FloatTraits {};

#if ARDUINOJSON_DOUBLE_IS_64BITS
template <typename T>
struct FloatTraits<T, 8 /*64bits*/> {
  typedef int64_t mantissa_type;
  static const short mantissa_bits = 52;
  static const mantissa_type mantissa_max =
      (static_cast<mantissa_type>(1) << mantissa_bits) - 1;

  typedef int16_t exponent_type;
  static const exponent_type exponent_max = 308;

  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0) {
      static T factors[] = {1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256};
      for (uint8_t index = 0; index < 9; e >>= 1, index++) {
        if (e & 1) m *= factors[index];
      }
    } else {
      e = TExponent(-e);
      static T factors[] = {1e-1,  1e-2,  1e-4,   1e-8,  1e-16,
                            1e-32, 1e-64, 1e-128, 1e-256};
      for (uint8_t index = 0; index < 9; e >>= 1, index++) {
        if (e & 1) m *= factors[index];
      }
    }
    return m;
  }

  static T nan() {
    return forge(0x7ff80000, 0x00000000);
  }

  static T inf() {
    return forge(0x7ff00000, 0x00000000);
  }

  static T forge(uint32_t msb, uint32_t lsb) {
    union {
      uint64_t integerBits;
      T floatBits;
    };
    integerBits = (uint64_t(msb) << 32) | lsb;
    return floatBits;
  }
};
#endif

template <typename T>
struct FloatTraits<T, 4 /*32bits*/> {
  typedef int32_t mantissa_type;
  static const short mantissa_bits = 23;
  static const mantissa_type mantissa_max =
      (static_cast<mantissa_type>(1) << mantissa_bits) - 1;

  typedef int8_t exponent_type;
  static const exponent_type exponent_max = 38;

  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0) {
      static T factors[] = {1e1f, 1e2f, 1e4f, 1e8f, 1e16f, 1e32f};
      for (uint8_t index = 0; e != 0; e >>= 1, index++) {
        if (e & 1) m *= factors[index];
      }
    } else {
      e = -e;
      static T factors[] = {1e-1f, 1e-2f, 1e-4f, 1e-8f, 1e-16f, 1e-32f};
      for (uint8_t index = 0; e != 0; e >>= 1, index++) {
        if (e & 1) m *= factors[index];
      }
    }
    return m;
  }

  static T forge(uint32_t bits) {
    union {
      uint32_t integerBits;
      T floatBits;
    };
    integerBits = bits;
    return floatBits;
  }

  static T nan() {
    return forge(0x7fc00000);
  }

  static T inf() {
    return forge(0x7f800000);
  }
};
}
}
