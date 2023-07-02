#include "Lexer.h"

Lexer::Lexer(const char* text)
{
	this->lineBuffer = (char*)text;
	this->currentChar = (char*)text;
}

Lexer::~Lexer(void)
{
}

void Lexer::clearToken()
{
	token =string();
}

bool Lexer::currentCharWhiteSpace()
{
	return (
				*currentChar == ' ' || 
				*currentChar == '	' || 
				*currentChar == ',' || 
				*currentChar == '(' || 
				*currentChar == ')' ||
				*currentChar == '\r' ||
				*currentChar == '\n'
		);
}

void Lexer::skipWhiteSpace()
{
	// C style comment, skipping rest of the line
	

	while(*currentChar && currentCharWhiteSpace()   )
	{
		if (*currentChar == '/' )
		{	
			if (*(currentChar+1) == '/')
				currentChar = lineBuffer + strlen(lineBuffer);
			
			return;
		}
		currentChar++;
	}
}

void Lexer::readToken()
{
	while(*currentChar && !currentCharWhiteSpace() )
	{
		//printf("Adding character ='%d', value = '%c'\n",*currentChar,*currentChar);
		token+=  *currentChar;
		currentChar++;
	}
	//printf("Token read = ' %s ' \n",token.c_str());
}

void Lexer::readReal()
{
	while(
			*currentChar && 
			(
				(
					(*currentChar) - '0' >= 0 && 
					(*currentChar) <= '9'
				)		
				||   (*currentChar)== '.' 
				|| (*currentChar)== '-'
			)
		 
			)
	{
		token+=  *currentChar;
		currentChar++;
	}
	//printf(" readReal = '%s'\n",token.c_str());
}

bool Lexer::done()
{
	return *currentChar == 0;
}