#pragma once

#include <string>

#ifdef _WIN32
#include <codecvt>
#endif

namespace util {

#ifdef _WIN32
inline std::wstring ConvertToNativeString(const std::string & str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wstr = converter.from_bytes(str);
	return wstr;
}
inline std::string ConvertFromNativeString(const std::wstring & str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string bstr = converter.to_bytes(str);
	return bstr;
}
#else
inline std::string ConvertToNativeString(const std::string & str) {
	return str;
}
inline std::string ConvertFromNativeString(const std::string & str) {
	return str;
}
#endif

/*
 * Searches for a module with a given name in the include path and
 * returns a relative path to it, if found.
 *
 * The directories checked are as follows:
 *
 *  0. If the path is complete, no searching is done
 *  1. The directory which contains the file performing the import.
 *  2. The project working directory
 *  3. The compiler's /lib directory.
 */
std::string FindIncludedFile(const std::string& name, const std::string& filedir, const std::string& libdir);

} // namespace util
