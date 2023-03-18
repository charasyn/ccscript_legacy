#pragma once
#ifdef _WIN32
	#ifndef _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	#endif // !_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem::v1;
#else
	#include "filesystem/filesystem_mini.h"
	namespace fs = filesystem;
#endif
