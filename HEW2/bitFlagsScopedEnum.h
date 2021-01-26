#pragma once

#include <type_traits>

//Tはintに変換できないenum、つまりenum class
template<typename T,
	typename std::enable_if<
	std::integral_constant<
	bool,
	std::is_enum<T>::value && !std::is_convertible<T, int>::value
	>::value,
	std::nullptr_t
	>::type = nullptr
>
constexpr inline T operator~ (T a) { return (T)~(int)a; }
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline T operator| (T a, T b) { return (T)((int)a | (int)b); }
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline T operator& (T a, T b) { return (T)((int)a & (int)b); }
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline T operator^ (T a, T b) { return (T)((int)a ^ (int)b); }
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline T& operator|= (T& a, T b) { return (T&)((int&)a |= (int)b); }
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline T& operator&= (T& a, T b) { return (T&)((int&)a &= (int)b); }
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline T& operator^= (T& a, T b) { return (T&)((int&)a ^= (int)b); }

//フラグがどちらかたってたらTRUE
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline bool operator+ (T a, T b) { return ((int)a | (int)b); }
//フラグが両方たってたらTRUE
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline bool operator* (T a, T b) { return ((int)a & (int)b); }
//フラグが片方だけたってたらTRUE
template<typename T, typename std::enable_if< std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>::value, std::nullptr_t>::type = nullptr>
constexpr inline bool operator% (T a, T b) { return ((int)a ^ (int)b); }