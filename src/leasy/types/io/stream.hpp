//
// Created by @wys on 13/08/2026.
//

#ifndef EASYRPG_PLAYER_ISTREAM_HPP
#define EASYRPG_PLAYER_ISTREAM_HPP

#include <iosfwd>
#include "../iterable.hpp"
#include "../spans.hpp"

namespace leasy {
  template<typename C, typename T1, typename T2>
  class BasicString;

  template <typename Char>
  class Streamable : public Iterable<std::streampos, Char>,
                     public SupportsSpan<Char> {
  public:
    using ptype = std::streampos;

    virtual Span<Char> read(size_t) = 0;
    virtual void write(const Span<Char>&) = 0;
    virtual void write(const BasicString<Char, std::char_traits<Char>, std::allocator<Char>>&) = 0;
    virtual ptype tellg() = 0;
    virtual ptype tellp() = 0;
    virtual void close() = 0;
    virtual void seekg(ptype) = 0;
    virtual void seekp(ptype) = 0;
  };

} // leasy

#endif //EASYRPG_PLAYER_ISTREAM_HPP
