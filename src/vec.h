#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <type_traits>
#include "util.h"

template <std::size_t N, Numeric T>
struct vec {
  static_assert(N >= 2, "Vector dimension must be >= 2");

  std::array<T, N> data{};

  constexpr T& operator[](std::size_t i) { return data[i]; }
  constexpr const T& operator[](std::size_t i) const { return data[i]; }

  constexpr vec() : data{} {}

  template <Numeric... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
  constexpr vec(Args... args) : data{static_cast<T>(args)...} {}

  constexpr vec(const vec&) = default;
  constexpr vec(vec&&) = default;
  constexpr vec& operator=(const vec&) = default;
  constexpr vec& operator=(vec&&) = default;
  constexpr bool operator==(const vec&) const = default;

  template <Numeric... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
  constexpr void set(Args... args) {
    std::size_t i = 0;
    ((data[i++] = static_cast<T>(args)), ...);
  }

  constexpr vec operator+(const vec& other) const {
    vec r;
    for (std::size_t i = 0; i < N; i++) {
      r[i] = data[i] + other[i];
    }
    return r;
  }

  constexpr vec operator-(const vec& other) const {
    vec r;
    for (std::size_t i = 0; i < N; i++) {
      r[i] = data[i] - other[i];
    }
    return r;
  }

  constexpr vec operator*(T scalar) const {
    vec r;
    for (std::size_t i = 0; i < N; i++) {
      r[i] = data[i] * scalar;
    }
    return r;
  }

  constexpr vec operator/(T scalar) const {
    vec r;
    for (std::size_t i = 0; i < N; i++) {
      r[i] = data[i] / scalar;
    }
    return r;
  }

  constexpr vec& operator+=(const vec& other) {
    for (std::size_t i = 0; i < N; i++) {
      data[i] += other[i];
    }
    return *this;
  }

  constexpr vec& operator-=(const vec& other) {
    for (std::size_t i = 0; i < N; i++) {
      data[i] -= other[i];
    }
    return *this;
  }

  constexpr vec& operator*=(T scalar) {
    for (std::size_t i = 0; i < N; i++) {
      data[i] *= scalar;
    }
    return *this;
  }

  constexpr vec& operator/=(T scalar) {
    for (std::size_t i = 0; i < N; i++) {
      data[i] /= scalar;
    }
    return *this;
  }

  constexpr T dot(const vec& other) const {
    T sum = T{};
    for (std::size_t i = 0; i < N; i++) {
      sum += data[i] * other[i];
    }
    return sum;
  }

  double length() const { return std::sqrt(static_cast<double>(dot(*this))); }

  vec<N, T> normalized() const {
    const auto len = length();
    if (len > T())
      return *this / len;
    return vec<N, T>{};
  }
};

template <Numeric T>
constexpr vec<3, T> cross(const vec<3, T>& a, const vec<3, T>& b) {
  return vec<3, T>{a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}

template <std::size_t N, Numeric T>
struct std::formatter<vec<N, T>> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
  auto format(const vec<N, T>& obj, std::format_context& ctx) const {
    std::string output = "(";
    for (std::size_t i = 0; i < N; i++) {
      output += std::format("{}", obj.data[i]);
      if (i != N - 1) {
        output += ", ";
      }
    }
    output += ")";
    return std::format_to(ctx.out(), "{}", output);
  }
};

template <std::size_t N, typename T>
struct std::hash<vec<N, T>> {
  size_t operator()(const vec<N, T>& v) const {
    size_t h = 0;
    for (auto& x : v.data) {
      h ^= std::hash<T>{}(x) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
  }
};

template <Numeric T>
using vec2 = vec<2, T>;

template <Numeric T>
using vec3 = vec<3, T>;

template <Numeric T>
using vec4 = vec<4, T>;
