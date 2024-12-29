/* exception class */
#pragma once

#include <string>

class Exception
{
protected:
	int num;
	std::string msg;

public:
	Exception(int num) : num(num), msg("") {}
	Exception(const std::string& msg) : num(-1), msg(msg) {}
	Exception(int num, const std::string& msg) : num(num), msg(msg) {}
	const std::string& GetMessage() const
	{
		return msg;
	}
};

