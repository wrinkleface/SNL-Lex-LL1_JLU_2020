#ifndef _SCANNER_H_
#define _SCANNER_H_
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream> 

using namespace std;
typedef enum
{
	ENDFILE, ERROR,
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END1, READ, WRITE, ARRAY, OF,
	RECORD, RETURN1,
	INTEGER, CHAR1,
	ID, INTC, CHARC,
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
}LexType;

static struct word {
	string str;   //语义信息
	LexType tok; //词法信息
}reservedWords[21] = { {"program",PROGRAM},{"type",TYPE},{"var",VAR},
	{"procedure",PROCEDURE},{"begin",BEGIN},{"end",END1},{"array",ARRAY},
	{"of",OF},{"record",RECORD},{"if",IF},{"then",THEN},{"else",ELSE},{"fi",FI},
	{"while",WHILE},{"do",DO},{"endwh",ENDWH},{"read",READ},{"write",WRITE},
	{"return",RETURN1},{"integer",INTEGER},{"char",CHAR1} };	//保留字


/*单词的TOKEN表示*/
struct token {
	int lineShow;	//在源程序中的行数
	struct word word;
};

class Scanner {

private:

	bool IsSeparater(char ch);

	bool IsOperator(char ch);

	bool IsKeyWord(string ch);

	bool IsFilter(char ch);

	bool IsLetter(char ch);

	bool IsDigit(char ch);

	token tokenList[10000];

	void WordAnalyse(FILE* fpin);

	LexType GetTokenType(string charList);

	struct word reservedLookup(string s);

	string convert(char chx);

	string toString(int lexType);

public:
	Scanner() {

	};

	token* getTokenList(FILE* fpin);

	void PrintTokenList();

};

#endif




