//
// Created by @wys on 13/08/2026.
//

#ifndef EASYRPG_PLAYER_FILE_HPP
#define EASYRPG_PLAYER_FILE_HPP

#include <fstream>
#include "stream.hpp"
#include "leasy/types/string.hpp"

namespace leasy {
  
  class File : public Streamable<char> {
  protected:
    mutable std::fstream stream;

  public:
    using ptype = typename Streamable<char>::ptype;

    Span<char> read(size_t size) override {
      Span<char> sp{size};
      stream.read(sp.data(), size);
      return sp;
    }

    void write(const Span<char> &span) override {
      stream.write(span.data(), span.size());
    }

    void write(const BasicString<char> &string) override {
      stream.write(string.data(), string.size());
    }

    ptype tellg() override {
      return stream.tellg();
    }

    ptype tellp() override {
      return stream.tellp();
    }

    void close() override {
      stream.close();
    }

    void seekg(ptype p) override {
      stream.seekg(p);
    }

    void seekp(ptype p) override {
      stream.seekp(p);
    }

    void forEach(const std::function<void(const ptype &, const char &)> &) const override {}

    void forEach(const std::function<void(const ptype&, char&)> &f) override {
      const auto lastpos = stream.tellg();

      stream.clear();
      stream.seekg(0, std::ios::beg);

      char c;
      uint64_t i{0};

      while (stream.get(c)) {
        f(i, c);
        ++i;
      }

      stream.clear();
      stream.seekg(lastpos);
    }

    bool hasValue(const char &) const override {
      return false;
    }

    bool hasIndex(const std::fpos<__mbstate_t> &) const override {
      return false;
    }

    [[nodiscard]] size_t size() const override {
      auto &s = const_cast<std::basic_fstream<char>&>(stream);

      const auto old = s.tellg();

      s.clear();
      s.seekg(0, std::ios::end);

      const auto end = s.tellg();

      s.clear();
      s.seekg(old);

      return static_cast<size_t>(end);
    }

    std::streampos begin() const override {
      return std::streampos{0};
    }

    void open(const std::filesystem::path &path, std::ios::openmode mode = std::ios::in | std::ios::out) const {
      stream.open(path, mode);
    }

    std::streampos end() const override {
      return size();
    }

    Span<char> asSpan() override {
      Span<char> span{size()};
      stream.seekg(0, std::ios::beg);
      stream.read(span.data(), size());

      return span;
    }

    Span<const char> asSpan() const override {
      Span<char> span{size()};
      stream.seekg(0, std::ios::beg);
      stream.read(span.data(), size());

      return span.makeReadOnly();
    }

    auto rdbuf() const {
      return stream.rdbuf();
    }

    ~File() override = default;
  };
} // leasy

#endif //EASYRPG_PLAYER_FILE_HPP
