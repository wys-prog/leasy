//
// Created by @wys on 13/08/2026.
//

#ifndef EASYRPG_PLAYER_SPANS_HPP
#define EASYRPG_PLAYER_SPANS_HPP

#include <stdexcept>
#include <algorithm>
#include <memory>

#include "iterable.hpp"

namespace leasy {
  /**
   * @brief Represents a view over an array of T, that you can modify. It also provides an access to the memory chunk
   * by giving a shared_ptr<T>, which allows you to reconstruct other iterators over the viewed element. (kinda dummy ig)
   * @tparam T element type
   */
  template<typename T>
  class Span : public Iterable<size_t, T> {
    T *ptr{nullptr};
    size_t length{0};

    bool owner{false};

  public:
    constexpr Span() = default;

    constexpr Span(T *ptr, size_t length)
      : ptr{ptr}, length{length} {
    }

    constexpr Span(T &value)
      : ptr{std::addressof(value)}, length{1} {
    }

    explicit Span(size_t l) : ptr(new T[l]), length{l}, owner{true} {}

    void forEach(const std::function<void(const size_t &, const T &)> &f) const override {
      for (size_t i = 0; i < length; ++i)
        f(i, ptr[i]);
    }
    void forEach(const std::function<void(const size_t &, T&)> &f) override {
      for (size_t i = 0; i < length; ++i)
        f(i, ptr[i]);
    }

    bool hasValue(const T &value) const override {
      return std::find(ptr, ptr + length, value) != ptr + length;
    }

    bool hasIndex(const size_t &i) const override {
      return i < length;
    }

    [[nodiscard]]
    size_t size() const override {
      return length;
    }

    T &operator[](size_t i) const {
      if (i >= length)
        throw std::out_of_range("Span index out of range");

      return ptr[i];
    }

    T *data() const {
      return ptr;
    }

    [[nodiscard]] size_t begin() const override {
      return 0;
    }

    size_t end() const override {
      return length;
    }

    ~Span() override {
      if (owner) {
        delete[] ptr;
      }
    }

    [[nodiscard]]
    Span<const T> makeReadOnly() const {
      return Span<const T>{ptr, length};
    }
  };

  template<typename T>
  class SupportsSpan {
  public:
    virtual ~SupportsSpan() = default;

    virtual Span<const T> asSpan() const = 0;
    virtual Span<T> asSpan() = 0;
  };
} // leasy

#endif //EASYRPG_PLAYER_SPANS_HPP
