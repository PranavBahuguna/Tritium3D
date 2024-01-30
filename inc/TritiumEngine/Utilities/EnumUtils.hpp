#pragma once

#include <type_traits>

// General template for enum flags
template <typename T> struct EnumFlags {};

// Partial specialization for enum class types to enable flags
#define ENABLE_ENUM_FLAGS(Enum)                                                                    \
  template <> struct EnumFlags<Enum> {                                                             \
    static constexpr bool enable = true;                                                           \
  };

// Bitwise NOT
template <typename T> typename std::enable_if<EnumFlags<T>::enable, T>::type operator~(T lhs) {
  using EnumT = typename std::underlying_type<T>::type;
  return static_cast<T>(~static_cast<EnumT>(lhs));
}

// Bitwise OR
template <typename T>
typename std::enable_if<EnumFlags<T>::enable, T>::type operator|(T lhs, T rhs) {
  using EnumT = typename std::underlying_type<T>::type;
  return static_cast<T>(static_cast<EnumT>(lhs) | static_cast<EnumT>(rhs));
}

// Bitwise AND
template <typename T>
typename std::enable_if<EnumFlags<T>::enable, T>::type operator&(T lhs, T rhs) {
  using EnumT = typename std::underlying_type<T>::type;
  return static_cast<T>(static_cast<EnumT>(lhs) & static_cast<EnumT>(rhs));
}

// Bitwise XOR
template <typename T>
typename std::enable_if<EnumFlags<T>::enable, T>::type operator^(T lhs, T rhs) {
  using EnumT = typename std::underlying_type<T>::type;
  return static_cast<T>(static_cast<EnumT>(lhs) ^ static_cast<EnumT>(rhs));
}

// Bitwise OR compound assignment
template <typename T>
typename std::enable_if<EnumFlags<T>::enable, T &>::type operator|=(T &lhs, T rhs) {
  lhs = lhs | rhs;
}

// Bitwise AND compound assignment
template <typename T>
typename std::enable_if<EnumFlags<T>::enable, T &>::type operator&=(T &lhs, T rhs) {
  lhs = lhs & rhs;
  return lhs;
}

// Bitwise XOR compound assignment
template <typename T>
typename std::enable_if<EnumFlags<T>::enable, T &>::type operator^=(T &lhs, T rhs) {
  lhs = lhs ^ rhs;
  return lhs;
}

// Bool cast helper function
template <typename T> typename std::enable_if<EnumFlags<T>::enable, bool>::type any(T x) {
  return static_cast<bool>(x);
}

// Inner type conversion helper function
template <typename T> constexpr inline decltype(auto) innerType(T x) {
  return static_cast<std::underlying_type_t<T>>(x);
}