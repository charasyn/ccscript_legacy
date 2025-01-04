/* implementation of ccscript strings */

#include "stringparser.h"
#include "ast.h"
#include "parser.h"
#include "module.h"
#include "bytechunk.h"
#include "utf8.h"
#include "util.h"

using std::string;


void StringParser::Error(const string &msg, int line_unused, int col)
{
	// We'll pass the error along to our internal error receiver,
	// but with a note added to indicate a string evaluation error
	error->Error(msg + " inside string", this->line, col);
}

void StringParser::Warning(const string &msg, int line_unused, int col)
{
	error->Warning(msg + " inside string", this->line, col);
}

int StringParser::acceptbyte()
{
	std::string s{};

	if(!util::IsHexDigit(current)) return -1;
	s += current;
	next();
	if(!util::IsHexDigit(current)) return -1;
	s += current;
	//next();

	unsigned int temp = 0;
	std::stringstream ss(s);
	ss >> std::setbase(16) >> temp;
	return static_cast<int>(temp);
}

void StringParser::next()
{
	if(strpos >= str.length()) {
		current = 0;
		return;
	}
	auto oldpos = strpos;
	current = utf8::utf8to32(str, strpos);
	if (current == utf8::DECODING_ERROR) {
		std::stringstream ss;
		ss << "invalid UTF-8 sequence '" << std::setbase(16);
		for (auto pos = oldpos; pos < strpos; ++pos) {
			ss << str[pos];
		}
		ss << "'";
		Error(ss.str(), 0, 0);
		current = 0;
	}
}

Value StringParser::Evaluate(SymbolTable* scope, EvalContext& context)
{
	String* output = new String();
	//ByteChunk* output = context.output;
	bool docodes = false;
	next();

	while(current != '\0') {
		// Handle '$' escapes
		if(current == '{') {
			output->Append( expression(scope, context).ToCodeString() );
			continue;
		}

		if(docodes) {
			// Break out of code mode
			if(current == ']') {
				next();
				docodes = false;
				continue;
			}
			// consume whitespace
			if(current == ' ' || current == '\t' || current == '\n') {
				next();
				continue;
			}
			int b = acceptbyte();
			if(b == -1)
				Warning(string("invalid control code bytes ignored"),0,0);
			else
				output->Byte(b);

			next();
		}
		else
		{
			if(current == '/') {
				output->Byte(16);
				output->Byte(5);
			}
			else if(current == '|') {
				output->Byte(16);
				output->Byte(15);
			}
			else if(current == '[') {
				docodes = true;
			}
			else {
				// Default:
				try {
					output->Char(current, context);
				} catch (const Exception & ex) {
					Error(ex.GetMessage(), 0, 0);
				}
			}
			next();
			continue;
		}
	}

	return Value(output);
}


Value StringParser::expression(SymbolTable* scope, EvalContext& context)
{
	// Create a parser on just this section of the string
	size_t n = str.find('}', strpos);

	if(n == string::npos) {
		Error(string("unterminated expression block"),0,0);
		strpos = n;
		next();
		return Value();
	}

	string exstr = str.substr(strpos, (n-strpos));

	Parser parser(exstr);
	parser.SetErrorHandler(this);

	// Parse one expression and evaluate it
	Expression* e = parser.ParseExpression();
	Value result = e->Evaluate(scope, context);

	// Skip the expression block
	strpos = n+1;
	next();

	return result;
}

