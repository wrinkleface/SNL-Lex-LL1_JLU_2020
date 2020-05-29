#pragma warning(disable:4996)；
#include <iostream>
//#include "Scanner.h"  //测试词法分析
#include "LL1.h"

using namespace std;

int main()
{
    FILE* fpin;//用于打开文件
    string file_name;//文件名

    /* //测试词法分析
    Scanner scanner;

    fpin = fopen("test_error.txt", "r");//根据文件名打开文件，打开方式是读取
    if (!fpin)//如果没有找到这个文件
    {
        cerr << "打开文件" << file_name << "失败" << endl;
        return -1;
    }

    token* s = scanner.getTokenList(fpin); //获取tokenlist
    scanner.PrintTokenList();   //输出tokenlist
    */

    LL1 l;
    //cout << "请输入文件名：";
    //cin >> file_name;//获取输入的文件名
    fpin = fopen("c1.txt", "r");//根据文件名打开文件，打开方式是读取
    if (!fpin)//如果没有找到这个文件
    {
        cerr << "打开文件" << file_name << "失败" << endl;
        return -1;
    }
    l.getResult(fpin); //输入需要分析的文件
    return 0;
}

