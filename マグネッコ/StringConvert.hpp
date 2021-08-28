#pragma once
#include "config.h"
#include <string>
//#include <winddi.h>
#include <vector>
#include <system_error>


		//https://nekko1119.hatenablog.com/entry/2017/01/02/054629
		//C++17から<convert>が非推奨なのでwinAPI使ってる
class StringConvert {
public:

	static std::wstring MultiToWide(std::string const& src)
	{
		auto const dest_size = ::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, nullptr, 0U);
		std::vector<wchar_t> dest(dest_size, L'\0');
		if (::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, dest.data(), dest.size()) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<wchar_t>::length(dest.data()));
		dest.shrink_to_fit();
		return std::wstring(dest.begin(), dest.end());
	}

	static std::string WideToUtf8(std::wstring const& src)
	{
		auto const dest_size = ::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
		std::vector<char> dest(dest_size, '\0');
		if (::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, dest.data(), dest.size(), nullptr, nullptr) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<char>::length(dest.data()));
		dest.shrink_to_fit();
		return std::string(dest.begin(), dest.end());
	}

	static std::string MultiToUtf8(std::string const& src)
	{
		auto const wide = MultiToWide(src);
		return WideToUtf8(wide);
	}
	static std::wstring Utf8ToWide(std::string const& src)
	{
		auto const dest_size = ::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, nullptr, 0U);
		std::vector<wchar_t> dest(dest_size, L'\0');
		if (::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, dest.data(), dest.size()) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<wchar_t>::length(dest.data()));
		dest.shrink_to_fit();
		return std::wstring(dest.begin(), dest.end());
	}
	static std::string WideToMulti(std::wstring const& src)
	{
		auto const dest_size = ::WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
		std::vector<char> dest(dest_size, '\0');
		if (::WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, dest.data(), dest.size(), nullptr, nullptr) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<char>::length(dest.data()));
		dest.shrink_to_fit();
		return std::string(dest.begin(), dest.end());
	}
	static std::string Utf8ToMulti(std::string const& src)
	{
		auto const wide = Utf8ToWide(src);
		return WideToMulti(wide);
	}
};

