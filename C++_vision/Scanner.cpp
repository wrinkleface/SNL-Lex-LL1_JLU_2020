#include "Scanner.h"

string Scanner::convert(char chx) {
	string ss = "";
	ss += chx;
	return ss;
}

/*�ж��Ƿ�Ϊ����*/
bool Scanner::IsDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*�ж��Ƿ�Ϊ��ĸ*/
bool Scanner::IsLetter(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*�ж��Ƿ�Ϊ���˷�*/
bool Scanner::IsFilter(char ch)
{

	if (ch == ' ')
	{
		return true;
	}
	else if (ch == '\t')
	{
		return true;
	}
	else if (ch == '\r')
	{
		return true;
	}
	else if (ch == '\n')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*�ж��Ƿ�Ϊ�ؼ���*/
bool Scanner::IsKeyWord(string ch)
{
	for (int i = 0; i < 21; i++)
	{
		if (ch == reservedWords[i].str)
		{
			return true;
		}
	}
	return false;
}

/*�ж��Ƿ�Ϊ�����*/
bool Scanner::IsOperator(char ch)
{
	if (ch == '+')
	{
		return true;
	}
	else if (ch == '-')
	{
		return true;
	}
	else if (ch == '*')
	{
		return true;
	}
	else if (ch == '/')
	{
		return true;
	}
	else if (ch == '<')
	{
		return true;
	}
	else if (ch == '=')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*�ж��Ƿ�Ϊ�ָ���*/
bool Scanner::IsSeparater(char ch)
{
	if (ch == ';')
	{
		return true;
	}
	else if (ch == ',')
	{
		return true;
	}
	else if (ch == '{')
	{
		return true;
	}
	else if (ch == '}')
	{
		return true;
	}
	else if (ch == '[')
	{
		return true;
	}
	else if (ch == ']')
	{
		return true;
	}
	else if (ch == '(')
	{
		return true;
	}
	else if (ch == ')')
	{
		return true;
	}
	else if (ch == '.')
	{
		return true;
	}
	else if (ch == '\'')
	{
		return true;
	}
	else if (ch == ':')
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*�����ֲ���*/
struct word Scanner::reservedLookup(string s)
{
	for (int i = 0; i < 21; i++)
	{
		if (reservedWords[i].str == s)
			return reservedWords[i];
	}
}


/*��ȡtoken���е�LexType�������ʵĴʷ���Ϣ*/
LexType Scanner::GetTokenType(string charList)
{
	LexType tokenType;
	if (charList == "+") {
		tokenType = PLUS;
	}
	else if (charList == "-") {
		tokenType = MINUS;
	}
	else if (charList == "*") {
		tokenType = TIMES;
	}
	else if (charList == "/") {
		tokenType = OVER;
	}
	else if (charList == "(") {
		tokenType = LPAREN;
	}
	else if (charList == ")") {
		tokenType = RPAREN;
	}
	else if (charList == ".") {
		tokenType = DOT;
	}
	else if (charList == "[") {
		tokenType = LMIDPAREN;
	}
	else if (charList == "]") {
		tokenType = RMIDPAREN;
	}
	else if (charList == ";") {
		tokenType = SEMI;
	}
	else if (charList == ":") {
		tokenType = COLON;
	}
	else if (charList == ",") {
		tokenType = COMMA;
	}
	else if (charList == "<") {
		tokenType = LT;
	}
	else if (charList == "=") {
		tokenType = EQ;
	}
	else if (charList == ":=") {
		tokenType = ASSIGN;
	}
	else if (charList == "..") {
		tokenType = UNDERANGE;
	}
	else if (charList == "EOF") {
		tokenType = ENDFILE;
	}
	else if (charList == "'") {
		tokenType = CHARC;
	}
	else {

		tokenType = ERROR;
	}
	return tokenType;
}

void Scanner::WordAnalyse(FILE* fpin)
{
	int lineShow = 1;	//ȷ����ʼ����
	int index = 0;		//tokenList��������
	char ch = fgetc(fpin);
	string arr = "";
	while (ch != EOF) {

		//�жϹ��˷�
		if (IsFilter(ch))
		{
			if (ch == '\n')
				lineShow += 1;
			ch = fgetc(fpin);
		}

		//�жϱ�ʶ����ؼ���
		else if (IsLetter(ch))
		{
			arr = "";
			arr = arr + ch;
			ch = fgetc(fpin);
			while (IsLetter(ch) || IsDigit(ch))
			{
				arr = arr + ch;
				ch = fgetc(fpin);
			}
			if (IsKeyWord(arr))
			{
				tokenList[index].lineShow = lineShow;
				tokenList[index].word = reservedLookup(arr);
				index++;
			}
			else
			{
				tokenList[index].lineShow = lineShow;
				tokenList[index].word.str = arr;
				tokenList[index].word.tok = ID;
				index++;
			}
		}

		//�ж������
		else if (IsOperator(ch))
		{
			LexType tok = GetTokenType(convert(ch));
			tokenList[index].lineShow = lineShow;
			tokenList[index].word.str = ch;
			tokenList[index].word.tok = tok;
			index++;
			ch = fgetc(fpin);
		}

		//�жϷָ���
		else if (IsSeparater(ch))
		{
			arr = "";
			//ȥ��ע��
			if (ch == '{')
			{
				while (ch != '}')
				{
					ch = fgetc(fpin);
					if (ch == '\n')
						lineShow += 1;
				}
				ch = fgetc(fpin);
			}
			//�ж������±��޽��
			else if (ch == '.')
			{
				arr += ch;
				if ((ch = fgetc(fpin)) == '.')
				{

					arr += ch;
					tokenList[index].lineShow = lineShow;
					tokenList[index].word.str = arr;
					tokenList[index].word.tok = GetTokenType(arr);
					index++;
					ch = fgetc(fpin);
				}
				else
				{
					tokenList[index].lineShow = lineShow;
					tokenList[index].word.str = arr;
					tokenList[index].word.tok = GetTokenType(arr);
					index++;
				}
			}
			//�ж��ַ���
			else if (ch == '\'')
			{
				tokenList[index].lineShow = lineShow;
				tokenList[index].word.tok = GetTokenType(convert(ch));
				while ((ch = fgetc(fpin)) != '\'')
				{
					arr += ch;
				}
				tokenList[index].word.str = arr;
				index++;
				ch = fgetc(fpin);
			}
			//�ж�˫�ַ��ֽ��
			else if (ch == ':')
			{
				arr += ch;
				if ((ch = fgetc(fpin)) == '=')
				{
					arr += ch;
					tokenList[index].lineShow = lineShow;
					tokenList[index].word.str = arr;
					tokenList[index].word.tok = GetTokenType(arr);
					index++;
					ch = fgetc(fpin);
				}
				else
				{
					tokenList[index].lineShow = lineShow;
					tokenList[index].word.str = arr;
					tokenList[index].word.tok = GetTokenType(arr);
					index++;
				}
			}
			else
			{
				tokenList[index].lineShow = lineShow;
				tokenList[index].word.str = convert(ch);
				tokenList[index].word.tok = GetTokenType(convert(ch));
				index++;
				ch = fgetc(fpin);
			}
		}

		//�ж��޷�������
		else if (IsDigit(ch))
		{
			arr = "";
			arr += ch;
			ch = fgetc(fpin);
			while (IsDigit(ch) || IsLetter(ch))
			{
				arr += ch;
				ch = fgetc(fpin);
			}
			int count = 0; //�ж��ַ����Ƿ����������
			for (int i = 0; i < arr.length(); i++)
			{
				count++;
				if (IsLetter(arr[i]))
				{
					tokenList[index].word.tok = ERROR;
					break;
				}
			}
			tokenList[index].lineShow = lineShow;
			tokenList[index].word.str = arr;
			if (count == arr.length())
				tokenList[index].word.tok = INTC;
			index++;
		}

		//δ֪����
		else
		{
			tokenList[index].lineShow = lineShow;
			tokenList[index].word.str = ch;
			tokenList[index].word.tok = GetTokenType(convert(ch));
			index++;
			ch = fgetc(fpin);
		}
	}
	tokenList[index].lineShow = lineShow;
	tokenList[index].word.str = ch;
	tokenList[index].word.tok = ENDFILE;
}


//��ʾ�ʷ��������
void Scanner::PrintTokenList()
{
	int i = 0;
	ofstream mycout0("tokenlist.txt");
	while (tokenList[i].word.tok != ENDFILE)
	{
		mycout0 << setw(4) << std::left << tokenList[i].lineShow << std::left << setw(25) << toString(tokenList[i].word.tok) << tokenList[i].word.str << endl;
		i++;
	}
	mycout0 << tokenList[i].lineShow << " " << toString(tokenList[i].word.tok) << " " << tokenList[i].word.str << endl;
	mycout0.close();
}

//ȡ�����е���
token* Scanner::getTokenList(FILE* fpin)
{
	WordAnalyse(fpin);
	return tokenList;
}

/*Lextypeת�����ַ������*/
string Scanner::toString(int lextype) {
	switch (lextype) {
	case 0:return "ENDFILE";
	case 1:return "ERROR";
		/*������*/
	case 2:return "PROGRAM";
	case 3:return "PROCEDURE";
	case 4:return "TYPE";
	case 5:return "VAR";
	case 6:return "IF";
	case 7:return "THEN";
	case 8:return "ELSE";
	case 9:return "FI";
	case 10:return "WHILE";
	case 11:return "DO";
	case 12:return "ENDWH";
	case 13:return "BEGIN";
	case 14:return "END";
	case 15:return "READ";
	case 16:return "WRITE";
	case 17:return "ARRAY";
	case 18:return "OF";
	case 19:return "RECORD";
	case 20:return "RETURN";
	case 21:return "INTEGER";
	case 22:return "CHAR";
		/*���ַ����ʷ���*/
	case 23:return "ID";
	case 24:return "INTC";
	case 25:return "CHARC";

		/*�������*/
	case 26:return "ASSIGN";
	case 27:return "EQ";
	case 28:return "LT";
	case 29:return "PLUS";
	case 30:return "MINUS";
	case 31:return "TIMES";
	case 32:return "OVER";
	case 33:return "LPAREN";
	case 34:return "RPAREN";
	case 35:return "DOT";
	case 36:return "COLON";
	case 37:return "SEMI";
	case 38:return "COMMA";
	case 39:return "LMIDPAREN";
	case 40:return "RMIDPAREN";
	case 41:return "UNDERANGE";
	}
}
