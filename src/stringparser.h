/* ccscript string routines */
#pragma once

#include "err.h"
#include <string>
#include <math.h>

#include "value.h"

class SymbolTable;
class Module;
class ByteChunk;
class EvalContext;

class StringParser : public ErrorReceiver
{
private:
	std::string str;
	size_t strpos;
	char32_t current;
	ErrorReceiver* error;

	int line;		// strictly for error reporting

public:
	StringParser(const std::string& str, int line, ErrorReceiver* e) :
		str(str), strpos(0), current(0), error(e), line(line) {}
	Value Evaluate(SymbolTable* scope, EvalContext& context);

	// ErrorReceiver implementation
	void Error(const std::string& msg, int line, int col);
	void Warning(const std::string& msg, int line, int col);

private:
	int acceptbyte();
	void next();
	Value expression(SymbolTable* scope, EvalContext& context);
};

