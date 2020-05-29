#include "Prepare.h"

bool Prepare::isNonterminal(string s) { // 判断字符串s是否为非终结符
	for (int i = 0; i < nonterminal.size(); i++)
	{
		if (nonTerminals(i) == s)
		{
			return true;
		}
	}
	return false;
}
int Prepare::getNIndex(string target) {  // 获得target在非终结符集合中的下标
	for (int i = 0; i < nonterminal.size(); i++) {

		if (target == nonTerminals(i))
			return i;
	}
	return -1;
}
int Prepare::getIndex(string target) {  // 获得target在终结符集合中的下标
	for (int i = 0; i < terminalNoEmpty.size(); i++) {
		if (target == tterminalNoEmpty(i))
			return i;
	}
	return -1;
}

//FirstSet和FollowSet相应下标的数组内，存放对应的非终结符的所有产生式的first集和follow集

// 求FIRST(target）
void Prepare::getFirst(string target)
{
	int countEmpty = 0;  // 用于最后判断是否有空
	int isEmpty = 0;
	int a = 0; //产生式右部长度 
	int targetIndex = getNIndex(target); // targetIndex等于target在非终结符集合中的下标

	for (int i = 0; i < T; i++)
	{
		//T=产生式条数

		// 例如 X->Y1..Yj..Yk 是一个产生式
		if ((nonTerminals(productions[i].left)) == target) // 匹配产生式左部
		{
			for (int j = 0; productions[i].right[j] != "0"; j++) {
				//cout<<productions[i].right[j]; 
				string Yj = productions[i].right[j]; //Yj是产生式右部

					/* ①Yj是终结符(不能产生空),FIRST(Yj)=Yj加入FIRST(X),结束*/
				if (!isNonterminal(Yj))
				{
					firstSet[targetIndex].insert(Yj);
					break;
				}
				/*②Yj是非终结符，递归，先求出FIRST(Yj)*/
				getFirst(Yj);
				set<string>::iterator it;
				int YjIndex = getNIndex(Yj); //YjIndex等于Yj在非终结符集合中的下标
				for (it = firstSet[YjIndex].begin(); it != firstSet[YjIndex].end(); it++)
				{
					if (*it == "$")  // 遍历查看FIRST(Yj)中是否含有'$'(能产生空)
						isEmpty = 1;
					else
					{
						firstSet[targetIndex].insert(*it);//将FIRST(Yj)中的非$就加入FIRST(X)
					}
				}
				if (isEmpty == 0)
				{
					//cout<<"插入00"<<isEmpty;  // Yj不能产生空, 迭代结束
					break;
				}
				else
				{
					//cout<<"插入："<<isEmpty;//  Yj能产生空
					countEmpty += isEmpty;
					isEmpty = 0;
				}
			}
			for (int j = 0; productions[i].right[j] != "0"; j++)
			{
				a += 1;   //判断产生式右部长度 
			}
			if (countEmpty == a)	//如果所有右部first(Y)都有$(空),将$加入FIRST(X)中
			{
				firstSet[getNIndex(target)].insert("$");
			}

		}
	}
}

// 求FOLLOW(target）
void Prepare::getFollow(string target, int aa[])
{
	static int rrecnum = 40;  //避免循环
	//static int aa[66];
	int ssss = 0;

	int targetIndex = getNIndex(target); //targetIndex等于target在非终结符集合中的下标
	if (aa[targetIndex] == 0)
	{
		//cout<<aa[targetIndex];
		aa[targetIndex] = 1;
	}
	else
	{
		//cout<<aa[targetIndex];
		return;
	}
	for (int i = 0; i < T; i++)
	{
		//	cout<<"i;   
		int index = -1;
		int len = 0;


		//计算产生式右部长度
		for (int j = 0; productions[i].right[j] != "0"; j++)
		{
			len++;
		}
		vector<int> allindex;
		// 寻找target在产生式中的位置index
		for (int j = 0; productions[i].right[j] != "0"; j++)
		{
			if (productions[i].right[j] == target)
			{
				/*index = j;
				break;*/
				allindex.push_back(j);
			}
			// else index=-1;
		}

		/*①
		找到target在产生式中的位置index
		存在A->αBβ, 将FIRST(β)中除了空$之外的所有加入FOLLOW(B)中
		这里B对应target, β对应nxt
		*/
		for (vector<int>::iterator iter = allindex.begin(); iter != allindex.end(); iter++)
		{
			index = *iter;

			if (index != -1 && index < len - 1)
			{
				int hasEmpty = 1;
				while (hasEmpty && (index < len - 1))
				{
					index = index + 1;
					string nxt = productions[i].right[index];
					if (!isNonterminal(nxt)) // β是终结符 FIRST(β)=β，直接插入β
					{
						followSet[targetIndex].insert(nxt);
						hasEmpty = 0;
						break;
					}
					else
					{  // β是非终结符
						hasEmpty = 0;
						set<string>::iterator it;
						int nxtIndex = getNIndex(nxt);  // 插入FIRST(β)中除了空$之外的所有
						for (it = firstSet[nxtIndex].begin(); it != firstSet[nxtIndex].end(); it++) {
							if (*it == "$")
								hasEmpty = 1;
							else
								followSet[targetIndex].insert(*it);
						}
					}
				}
				/*②
				存在A->αBβ且FIRST(β)->$
				FOLLOW(A)放在FOLLOW(B)中
				*/
				if (hasEmpty && nonTerminals(productions[i].left) != target)
				{
					getFollow(nonTerminals(productions[i].left), aa);
					set<string>::iterator it;
					string tmp = nonTerminals(productions[i].left);
					int tmpIndex = getNIndex(tmp);
					for (it = followSet[tmpIndex].begin(); it != followSet[tmpIndex].end(); it++)
						followSet[targetIndex].insert(*it);
				}
			}
			/*③
			存在A->αB ,FOLLOW(A)放在FOLLOW(B)中
			*/
			else if (index != -1 && index == len - 1 && target != nonTerminals(productions[i].left))
			{
				getFollow(nonTerminals(productions[i].left), aa);
				set<string>::iterator it2;
				string tmp = nonTerminals(productions[i].left);
				int tmpIndex = getNIndex(tmp);
				for (it2 = followSet[tmpIndex].begin(); it2 != followSet[tmpIndex].end(); it2++)
				{
					followSet[targetIndex].insert(*it2);
				}

			}
		}
	}

}

//输出FIRST集、FOLLOW集
void Prepare::getFirstAndFollow()
{
	ofstream mycout1("first.txt");
	for (int i = 0; i < nonterminal.size(); i++)
	{
		set<string>::iterator it;
		string target = (nonTerminals(i));
		getFirst(target);
		mycout1 << "First(" << std::left << setw(15) << target << ")=";
		for (it = firstSet[i].begin(); it != firstSet[i].end(); it++)
		{
			mycout1 << std::left << setw(10) << *it;
		}
		mycout1 << '\n';
	}
	mycout1.close();

	ofstream mycout2("follow.txt");
	for (int i = 0; i < nonterminal.size(); i++)
	{
		string target = (nonTerminals(i));
		mycout2 << "Follow(" << std::left << setw(15) << target << ")=";
		int aa[67];
		for (int j = 0; j < 67; j++) {
			aa[j] = 0;
		}
		getFollow(target, aa);
		int x = 0;
		set<string>::iterator it;
		for (it = followSet[i].begin(); it != followSet[i].end(); it++) {
			x++;
			mycout2 << std::left << setw(10) << *it;
		}
		mycout2 << '\n';
	}
	mycout2.close();
}

//枚举转换为字符串
string Prepare::nonTerminals(int n)
{
	switch (n) {
	case 0:return "Program";
	case 1:return "ProgramHead";
	case 2:return "ProgramName";
	case 3:return "DeclarePart";
	case 4:return "TypeDec";
	case 5:return "TypeDeclaration";
	case 6:return "TypeDecList";
	case 7:return "TypeDecMore";
	case 8:return "TypeId";
	case 9:return "TypeName";
	case 10:return "BaseType";
	case 11:return "StructureType";
	case 12:return "ArrayType";
	case 13:return "Low";
	case 14:return "Top";
	case 15:return "RecType";
	case 16:return "FieldDecList";
	case 17:return "FieldDecMore";
	case 18:return "IdList";
	case 19:return "IdMore";
	case 20:return "VarDec";
	case 21:return "VarDeclaration";
	case 22:return "VarDecList";
	case 23:return "VarDecMore";
	case 24:return "VarIdList";
	case 25:return "VarIdMore";
	case 26:return "ProcDec";
	case 27:return "ProcDeclaration";
	case 28:return "ProcDecMore";
	case 29:return "ProcName";
	case 30:return "ParamList";
	case 31:return "ParamDecList";
	case 32:return "ParamMore";
	case 33:return "Param";
	case 34:return "FormList";
	case 35:return "FidMore";
	case 36:return "ProcDecPart";
	case 37:return "ProcBody";
	case 38:return "ProgramBody";
	case 39:return "StmList";
	case 40:return "StmMore";
	case 41:return "Stm";
	case 42:return "AssCall";
	case 43:return "AssignmentRest";
	case 44:return "ConditionalStm";
	case 45:return "LoopStm";
	case 46:return "InputStm";
	case 47:return "Invar";
	case 48:return "OutputStm";
	case 49:return "ReturnStm";
	case 50:return "CallStmRest";
	case 51:return "ActParamList";
	case 52:return "ActParamMore";
	case 53:return "RelExp";
	case 54:return "OtherRelE";
	case 55:return "Exp";
	case 56:return "OtherTerm";
	case 57:return "Term";
	case 58:return "OtherFactor";
	case 59:return "Factor";
	case 60:return "Variable";
	case 61:return "VariMore";
	case 62:return "FieldVar";
	case 63:return "FieldVarMore";
	case 64:return "CmpOp";
	case 65:return "AddOp";
	case 66:return "MultOp";
	}
};
string Prepare::tterminalNoEmpty(int l) {
	switch (l) {
	case 0:return "ENDFILE";
	case 1:return "ERROR";
		/*保留字*/
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
		/*多字符单词符号*/
	case 23:return "ID";
	case 24:return "INTC";
	case 25:return "CHARC";
		/*特殊符号*/
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