#pragma once
#include <functional>
#include <optional>
#include <vector>
#include "util.h"

template <typename T>
class Matrix {
 public:
  Matrix() = default;
  Matrix(std::vector<std::vector<T>>&& data) { set_data(std::move(data)); }
  ~Matrix() = default;

  Matrix(const Matrix& other) = default;
  Matrix(Matrix&&) = default;

  inline void set_data(const std::vector<std::vector<T>>& data) noexcept {
    m_data = data;
    update_dims();
  }

  inline void set_data(std::vector<std::vector<T>>&& data) noexcept {
    m_data = std::move(data);
    update_dims();
  }

  inline size_t width() const noexcept { return m_width; }
  inline size_t height() const noexcept { return m_height; }

  const T* get_ptr(size_t x, size_t y) const noexcept {
    if (!within(x, 0, m_width, LEFT_INCLUSIVE) || !within(y, 0, m_height, LEFT_INCLUSIVE)) {
      return nullptr;
    }
    return m_data.at(y).at(x);
  }

  std::optional<std::reference_wrapper<const T>> get_ref(size_t x, size_t y) const noexcept {
    if (!within(x, 0, m_width, LEFT_INCLUSIVE) || !within(y, 0, m_height, LEFT_INCLUSIVE)) {
      return std::nullopt;
    }
    return std::cref(m_data.at(y).at(x));
  }

  std::optional<T> get(size_t x, size_t y) const noexcept {
    if (!within(x, 0, m_width, LEFT_INCLUSIVE) || !within(y, 0, m_height, LEFT_INCLUSIVE)) {
      return {};
    }
    return m_data.at(y).at(x);
  }

  inline std::vector<std::vector<T>> get_data() const noexcept { return m_data; }

 private:
  inline void update_dims() noexcept {
    m_width = m_data.size() > 0 ? m_data.at(0).size() : 0;
    m_height = m_data.size();
  }

  std::vector<std::vector<T>> m_data;
  size_t m_width = 0;
  size_t m_height = 0;
};

template <typename T>
struct std::formatter<Matrix<T>> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
  auto format(const Matrix<T>& obj, std::format_context& ctx) const {
    auto out = ctx.out();

    for (size_t i = 0; i < obj.height(); ++i) {
      for (size_t j = 0; j < obj.width(); ++j) {
        out = std::format_to(out, "{}", obj.get(j, i).value());
      }
      *out++ = '\n';
    }
    return out;
  }
};