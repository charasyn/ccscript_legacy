#include "utf8.h"
#include <codecvt>
#include <iomanip>
#include <locale>
#include <sstream>

char32_t utf8::utf8to32(const std::string &str, size_t &strOffset) {
	char32_t outputArr[2] = {};
	const char *const inputStartPtr = str.data() + strOffset;
	const char *const inputEndPtr = str.data() + str.length();
	const char *inputNextPtr = nullptr;
	char32_t *outputNextPtr = nullptr;
	const auto &locale = std::locale::classic();
	const auto &facet = std::use_facet<std::codecvt<char32_t, char, std::mbstate_t>>(locale);
	std::mbstate_t state{};
	auto result = facet.in(state, inputStartPtr, inputEndPtr, inputNextPtr,
	                              &outputArr[0], &outputArr[1], outputNextPtr);
	if ((result == std::codecvt_base::ok || result == std::codecvt_base::partial)) {
		strOffset = inputNextPtr - str.data();
		// If output character is outside of Unicode range, return invalid.
		return outputArr[0] <= 0x10FFFF ? outputArr[0] : DECODING_ERROR;
	} else {
		// Error while decoding, return invalid.
		strOffset += 1;
		return DECODING_ERROR;
	}
}
std::string utf8::utf32to8(char32_t codepoint) {
	// This isn't directly generated user data, so we should raise an exception
	// for encoding errors.
	if (codepoint > 0x10FFFF) {
		std::stringstream ss;
		ss << "Invalid Unicode codepoint 0x" << std::setbase(16) << codepoint;
		throw Exception(ss.str());
	}
	char32_t inputArr[2] = {codepoint, 0};
	char outputArr[8] = {};
	const char32_t * const inputStartPtr = &inputArr[0];
	const char32_t * const inputEndPtr = &inputArr[1];
	const char32_t * inputNextPtr = nullptr;
	char * const outputStartPtr = &outputArr[0];
	char * const outputEndPtr = &outputArr[7];
	char * outputNextPtr = nullptr;
	const auto &locale = std::locale::classic();
	const auto &facet = std::use_facet<std::codecvt<char32_t, char, std::mbstate_t>>(locale);
	std::mbstate_t state{};
	auto result = facet.out(state, inputStartPtr, inputEndPtr, inputNextPtr,
	                               outputStartPtr, outputEndPtr, outputNextPtr);
	if ((result == std::codecvt_base::ok || result == std::codecvt_base::partial)) {
		return std::string(outputStartPtr, outputNextPtr);
	} else {
		// Error while encoding, raise exception
		std::stringstream ss;
		ss << "Error while encoding Unicode codepoint 0x" << std::setbase(16) << codepoint;
		throw Exception(ss.str());
	}
}
