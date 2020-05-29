#ifndef _LL1_H_
#define _LL1_H_
#include"Prepare.h"
#include<vector>
#include<set>
#define NonTerminalsSize 67
#define terminalNoEmptySize 42
class LL1 : public Prepare
{
private:
    vector<string> analyStack; // 分析栈
    vector<string> leftExpr;  // 剩余输入串
    int LL1Table[NonTerminalsSize][terminalNoEmptySize]; //LL1分析表
public:
    LL1()
    {
        for (int i = 0; i < NonTerminalsSize; i++) {
            for (int j = 0; j < terminalNoEmptySize; j++) {
                LL1Table[i][j] = -1;   //LL1Table初始化
            }
        }
    }
    void CreatLL1Table(); //得到LL1分析表
    void analyExpression(struct token s[]);  // 分析输入语句s
    void printPredictTable();  // 输出预测表
    int IsScanner(struct token s[]); //词法分析是否成功
    void getResult(FILE* fpin); // 综合处理,得出结果
};
#endif




