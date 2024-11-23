#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<map>
using std::cout;
using std::cin;
using std::endl;

//static enum Operations
//{
//	Plus,
//	Minus,
//	Aster,
//	Slash
//};

const std::map<int, std::string> Operations =
{
	{0, "Plus" },
	{1, "Minus" },
	{2, "Aster" },
	{3, "Slash" }
};

int main()
{

	char sz_digigt[2] = "0";
	const int SIZE = 256;
	for (int i = 0; i < 4; i++)
	{
		std::string test = Operations.at(i);
		std::string str1 = "ButtonsBMP\\square_blue\\button_";
		std::string test2 = str1 + test;
		cout << test2 << endl;;
	}


	cout << endl;
}
