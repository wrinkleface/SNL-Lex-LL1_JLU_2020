#include "LL1.h"

/*产生LL1分析表
    LL1Table[NonTerminalsSize][terminalNoEmptySize]
    产生式非终结符作为行，产生式对应Predict集中每个元素作为列
    设有产生式(编号index):A->α
    ①若a∈Predict（A->α），则LL1Table[A][a]=index
    ②a不属于任何一个以A为左部的产生式的Predict集，则LL1Table[A][a]=-1
*/
void LL1::CreatLL1Table()
{
    getFirstAndFollow(); //先调用该函数获得first集和follow集
    for (int index = 0; index < T; index++)
    {  						 // 对于每个产生式(编号index):A->α
        int row = getNIndex(nonTerminals(productions[index].left));
        int emptyCount = 0;
        // 1) Predict（A->α）=FIRST(α)。
        for (int i = 0; productions[index].right[i] != "0"; i++)
        {
            //tmp是产生式index的右部
            string tmp = productions[index].right[i];
            if (!isNonterminal(tmp) || tmp == "$")
            {
                // tmp是终结符。对FIRST(α)中的每个终结符号α,LL1Table[A][a]=index
                if (tmp != "$")
                    LL1Table[row][getIndex(tmp)] = index; //LL1Table[A][a]=index
                if (tmp == "$")
                {
                    emptyCount++;
                }
                break;
            }
            else
            {
                // tmp是非终结符
                set<string>::iterator it;
                int tmpIndex = getNIndex(tmp);
                // 对FIRST(tmp)中的每个终结符号α,LL1Table[A][a]=index
                for (it = firstSet[tmpIndex].begin(); it != firstSet[tmpIndex].end(); it++)
                {
                    LL1Table[row][getIndex(*it)] = index;//LL1Table[A][a]=index
                }
                if (firstSet[tmpIndex].count("$") != 0)
                {
                    // 如果空$在FIRST(tmp)中,继续看α中的下一个符号
                    emptyCount++;
                }
                else
                {
                    break;
                }
            }
        }
        // 2) Predict（A->α）={FIRST(α)-{$}}∪FOLLOW(A)。
        //如果空$在FIRST(α)中,对FOLLOW(A)中的每个终结符或结束符b,LL1Table[A][a]=index
        if (emptyCount == rlen(productions[index].right))
        {
            set<string>::iterator  it;
            for (it = followSet[row].begin(); it != followSet[row].end(); it++)
            {
                LL1Table[row][getIndex(*it)] = index;   // LL1Table[A][a]=index
            }
        }
    }
}
/*
1.分析斩栈顶是终结符，则看其是否与输入流的头符相匹配。如果匹配成功，则去掉栈顶元素并读入下一单词。若不成功，则报错
2.栈顶是非终结符，则用栈顶和输入流的当前单词去查LL1Table。如查值是产生式编号，则把对应产生式右部逆序压入栈中；如查值是错误信息，则报错
3.栈已空，输入流不空，输入流报错
4.栈已空，输入流已空，语法分析成功
*/
void LL1::analyExpression(struct token s[])
{
    ofstream mycout4("分析过程.txt");
    vector<int> lineStack;
    for (int i = 0; s[i].word.tok != ENDFILE; i++)
    {
        leftExpr.push_back(tterminalNoEmpty(s[i].word.tok));
        lineStack.push_back(s[i].lineShow);
    }
    leftExpr.push_back("#");
    analyStack.push_back("#");
    analyStack.push_back(nonTerminals(nonterminal[0]));  // 加入开始符号

    while (analyStack.size() > 0)
    {
        //cout<<"分析栈：";
        string outs = "";
        for (int i = 0; i < analyStack.size(); i++)
            outs += analyStack[i];
        mycout4 << endl;
        mycout4 << "分析栈:" << endl;
        mycout4 << "+++++++++++++++++++++++++++++++++++++" << endl;
        mycout4 << setw(15) << outs << endl;
        mycout4 << "+++++++++++++++++++++++++++++++++++++" << endl;

        //cout<<"剩余输入串：";
        outs = "";
        for (int i = 0; i < leftExpr.size(); i++)
            outs = outs + leftExpr[i] + "  ";
        mycout4 << endl;
        mycout4 << "剩余输入串:" << endl;
        mycout4 << "*************************************" << endl;
        mycout4 << setw(15) << outs << endl;
        mycout4 << "*************************************" << endl;

        // 匹配--分析斩栈顶是否与输入流栈的头符相匹配
        string string1 = analyStack.back();
        string string2 = leftExpr.front();
        //4.栈已空，输入流已空，语法分析成功
        if (string1 == string2 && string1 == "#")
        {
            mycout4 << setw(15) << "Accepted!" << endl << setw(15) << "语法分析成功" << endl;
            return;
        }
        //1.分析斩栈顶是终结符，则看其是否与输入流的头符相匹配。
        if (string1 == string2)
        {
            //匹配成功，则去掉栈顶元素并读入下一单词。
            analyStack.pop_back();
            leftExpr.erase(leftExpr.begin());
            lineStack.erase(lineStack.begin());
            mycout4 << "匹配：" << string1 << endl;
        }
        //2.栈顶是非终结符，则用栈顶和输入流的当前单词去查LL1Table。；如查值是错误信息，则报错
        else if (LL1Table[getNIndex(string1)][getIndex(string2)] != -1)
        {  // 预测表中有推导项，可进行推导
            int tg = LL1Table[getNIndex(string1)][getIndex(string2)];
            analyStack.pop_back();

            if (productions[tg].right[0] != "$")
            {
                //如查值是产生式编号，则把对应产生式右部逆序压入栈中
                for (int i = rlen(productions[tg].right) - 1; i >= 0; i--)
                { // 注意这里是反向的，逆序压入
                    analyStack.push_back(productions[tg].right[i]);
                }
            }
            string str = "";
            for (int i = 0; i < rlen(productions[tg].right); i++)
            {
                str = str + productions[tg].right[i] + "  ";
            }
            mycout4 << "推导：" << nonTerminals(productions[tg].left) << "->" << str << endl;
        }
        else
        {  // 错误
            mycout4 << setw(15) << "error!" << endl;
            mycout4 << "第" << lineStack.front() << "行" << "出错" << setw(10) << "应为" << s[lineStack.front()].word.str;
            return;
        }
    }
    mycout4.close();
}

void LL1::printPredictTable()
{
    // 表头
    ofstream mycout5("Predict.txt");
    mycout5 << std::left << setw(16) << "NonT/T";
    for (int k = 0; k < terminalNoEmpty.size(); k++)
    {
        mycout5 << std::left << setw(10) << tterminalNoEmpty(terminalNoEmpty[k]);
    }
    for (int i = 0; i < nonterminal.size(); i++)
    {
        mycout5 << endl;
        mycout5 << std::left << setw(15) << nonTerminals(nonterminal[i]) << std::left << setw(1) << ":";
        for (int j = 0; j < terminalNoEmpty.size(); j++)
        {
            if (LL1Table[i][j] == -1)
                mycout5 << std::left << setw(10) << "X";
            else
            {
                mycout5 << std::left << setw(10) << LL1Table[i][j];
                /*for (int n = 0; productions[LL1Table[i][j]].right[n]!= "0"; n++)
                {
                    mycout5 << productions[LL1Table[i][j]].right[n];
                }*/
            }
        }
    }
    mycout5.close();
}
int LL1::IsScanner(struct token s[])
{
    int error_flag = 0;
    for (int k = 0; s[k].word.tok != ENDFILE; k++)
    {
        if (s[k].word.tok == ERROR)
        {
            error_flag += 1;
        }
    }
    return error_flag;
}
void LL1::getResult(FILE* fpin)
{
    int ScannerFlag;
    Scanner scanner;
    token* s = scanner.getTokenList(fpin); //获取tokenlist
    ScannerFlag = IsScanner(s);
    if (ScannerFlag == 0)
    {
        scanner.PrintTokenList();   //输出tokenlist
        CreatLL1Table();           //创建LL1表
        printPredictTable();       //输出predict表
        //栈匹配
        analyExpression(s);        //分析tokenlist
    }
    else
    {
        cout << "词法分析出" << ScannerFlag << "处错误！不能继续语法分析" << endl;
    }
}

