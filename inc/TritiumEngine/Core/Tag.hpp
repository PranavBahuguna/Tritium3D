#pragma once

#include <cstring>
#include <string>

namespace TritiumEngine::Core
{
  class Tag {
  public:
    Tag(const char *name) : m_name(name) {}
    Tag(const std::string &name) : m_name(name) {}

    friend bool operator==(const Tag &lhs, const Tag &rhs) { return lhs.m_name == rhs.m_name; }

  private:
    std::string m_name;
  };

  const static Tag DEFAULT_TAG = "";
} // namespace TritiumEngine::Core