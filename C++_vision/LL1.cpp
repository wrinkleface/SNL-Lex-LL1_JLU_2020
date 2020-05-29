#include "LL1.h"

/*����LL1������
    LL1Table[NonTerminalsSize][terminalNoEmptySize]
    ����ʽ���ս����Ϊ�У�����ʽ��ӦPredict����ÿ��Ԫ����Ϊ��
    ���в���ʽ(���index):A->��
    ����a��Predict��A->��������LL1Table[A][a]=index
    ��a�������κ�һ����AΪ�󲿵Ĳ���ʽ��Predict������LL1Table[A][a]=-1
*/
void LL1::CreatLL1Table()
{
    getFirstAndFollow(); //�ȵ��øú������first����follow��
    for (int index = 0; index < T; index++)
    {  						 // ����ÿ������ʽ(���index):A->��
        int row = getNIndex(nonTerminals(productions[index].left));
        int emptyCount = 0;
        // 1) Predict��A->����=FIRST(��)��
        for (int i = 0; productions[index].right[i] != "0"; i++)
        {
            //tmp�ǲ���ʽindex���Ҳ�
            string tmp = productions[index].right[i];
            if (!isNonterminal(tmp) || tmp == "$")
            {
                // tmp���ս������FIRST(��)�е�ÿ���ս���Ŧ�,LL1Table[A][a]=index
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
                // tmp�Ƿ��ս��
                set<string>::iterator it;
                int tmpIndex = getNIndex(tmp);
                // ��FIRST(tmp)�е�ÿ���ս���Ŧ�,LL1Table[A][a]=index
                for (it = firstSet[tmpIndex].begin(); it != firstSet[tmpIndex].end(); it++)
                {
                    LL1Table[row][getIndex(*it)] = index;//LL1Table[A][a]=index
                }
                if (firstSet[tmpIndex].count("$") != 0)
                {
                    // �����$��FIRST(tmp)��,���������е���һ������
                    emptyCount++;
                }
                else
                {
                    break;
                }
            }
        }
        // 2) Predict��A->����={FIRST(��)-{$}}��FOLLOW(A)��
        //�����$��FIRST(��)��,��FOLLOW(A)�е�ÿ���ս���������b,LL1Table[A][a]=index
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
1.����նջ�����ս���������Ƿ�����������ͷ����ƥ�䡣���ƥ��ɹ�����ȥ��ջ��Ԫ�ز�������һ���ʡ������ɹ����򱨴�
2.ջ���Ƿ��ս��������ջ�����������ĵ�ǰ����ȥ��LL1Table�����ֵ�ǲ���ʽ��ţ���Ѷ�Ӧ����ʽ�Ҳ�����ѹ��ջ�У����ֵ�Ǵ�����Ϣ���򱨴�
3.ջ�ѿգ����������գ�����������
4.ջ�ѿգ��������ѿգ��﷨�����ɹ�
*/
void LL1::analyExpression(struct token s[])
{
    ofstream mycout4("��������.txt");
    vector<int> lineStack;
    for (int i = 0; s[i].word.tok != ENDFILE; i++)
    {
        leftExpr.push_back(tterminalNoEmpty(s[i].word.tok));
        lineStack.push_back(s[i].lineShow);
    }
    leftExpr.push_back("#");
    analyStack.push_back("#");
    analyStack.push_back(nonTerminals(nonterminal[0]));  // ���뿪ʼ����

    while (analyStack.size() > 0)
    {
        //cout<<"����ջ��";
        string outs = "";
        for (int i = 0; i < analyStack.size(); i++)
            outs += analyStack[i];
        mycout4 << endl;
        mycout4 << "����ջ:" << endl;
        mycout4 << "+++++++++++++++++++++++++++++++++++++" << endl;
        mycout4 << setw(15) << outs << endl;
        mycout4 << "+++++++++++++++++++++++++++++++++++++" << endl;

        //cout<<"ʣ�����봮��";
        outs = "";
        for (int i = 0; i < leftExpr.size(); i++)
            outs = outs + leftExpr[i] + "  ";
        mycout4 << endl;
        mycout4 << "ʣ�����봮:" << endl;
        mycout4 << "*************************************" << endl;
        mycout4 << setw(15) << outs << endl;
        mycout4 << "*************************************" << endl;

        // ƥ��--����նջ���Ƿ���������ջ��ͷ����ƥ��
        string string1 = analyStack.back();
        string string2 = leftExpr.front();
        //4.ջ�ѿգ��������ѿգ��﷨�����ɹ�
        if (string1 == string2 && string1 == "#")
        {
            mycout4 << setw(15) << "Accepted!" << endl << setw(15) << "�﷨�����ɹ�" << endl;
            return;
        }
        //1.����նջ�����ս���������Ƿ�����������ͷ����ƥ�䡣
        if (string1 == string2)
        {
            //ƥ��ɹ�����ȥ��ջ��Ԫ�ز�������һ���ʡ�
            analyStack.pop_back();
            leftExpr.erase(leftExpr.begin());
            lineStack.erase(lineStack.begin());
            mycout4 << "ƥ�䣺" << string1 << endl;
        }
        //2.ջ���Ƿ��ս��������ջ�����������ĵ�ǰ����ȥ��LL1Table�������ֵ�Ǵ�����Ϣ���򱨴�
        else if (LL1Table[getNIndex(string1)][getIndex(string2)] != -1)
        {  // Ԥ��������Ƶ���ɽ����Ƶ�
            int tg = LL1Table[getNIndex(string1)][getIndex(string2)];
            analyStack.pop_back();

            if (productions[tg].right[0] != "$")
            {
                //���ֵ�ǲ���ʽ��ţ���Ѷ�Ӧ����ʽ�Ҳ�����ѹ��ջ��
                for (int i = rlen(productions[tg].right) - 1; i >= 0; i--)
                { // ע�������Ƿ���ģ�����ѹ��
                    analyStack.push_back(productions[tg].right[i]);
                }
            }
            string str = "";
            for (int i = 0; i < rlen(productions[tg].right); i++)
            {
                str = str + productions[tg].right[i] + "  ";
            }
            mycout4 << "�Ƶ���" << nonTerminals(productions[tg].left) << "->" << str << endl;
        }
        else
        {  // ����
            mycout4 << setw(15) << "error!" << endl;
            mycout4 << "��" << lineStack.front() << "��" << "����" << setw(10) << "ӦΪ" << s[lineStack.front()].word.str;
            return;
        }
    }
    mycout4.close();
}

void LL1::printPredictTable()
{
    // ��ͷ
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
    token* s = scanner.getTokenList(fpin); //��ȡtokenlist
    ScannerFlag = IsScanner(s);
    if (ScannerFlag == 0)
    {
        scanner.PrintTokenList();   //���tokenlist
        CreatLL1Table();           //����LL1��
        printPredictTable();       //���predict��
        //ջƥ��
        analyExpression(s);        //����tokenlist
    }
    else
    {
        cout << "�ʷ�������" << ScannerFlag << "�����󣡲��ܼ����﷨����" << endl;
    }
}

