#pragma warning(disable:4996)��
#include <iostream>
//#include "Scanner.h"  //���Դʷ�����
#include "LL1.h"

using namespace std;

int main()
{
    FILE* fpin;//���ڴ��ļ�
    string file_name;//�ļ���

    /* //���Դʷ�����
    Scanner scanner;

    fpin = fopen("test_error.txt", "r");//�����ļ������ļ����򿪷�ʽ�Ƕ�ȡ
    if (!fpin)//���û���ҵ�����ļ�
    {
        cerr << "���ļ�" << file_name << "ʧ��" << endl;
        return -1;
    }

    token* s = scanner.getTokenList(fpin); //��ȡtokenlist
    scanner.PrintTokenList();   //���tokenlist
    */

    LL1 l;
    //cout << "�������ļ�����";
    //cin >> file_name;//��ȡ������ļ���
    fpin = fopen("c1.txt", "r");//�����ļ������ļ����򿪷�ʽ�Ƕ�ȡ
    if (!fpin)//���û���ҵ�����ļ�
    {
        cerr << "���ļ�" << file_name << "ʧ��" << endl;
        return -1;
    }
    l.getResult(fpin); //������Ҫ�������ļ�
    return 0;
}

