#pragma once

#include "includes.h"

class Lexer
{
public:
	Lexer(const char* text);
	~Lexer(void);

	string token;

	 void skipWhiteSpace();
	 void readToken();
	 void readReal();
	 void clearToken();
	 bool done();

private :
	char* currentChar;
	char*  lineBuffer;
	bool currentCharWhiteSpace();

};
