#include<iostream>


using std::cout;
using std::cin;
using std::endl;

int main()
{
	unsigned int a;
	cin >> a;
	int b = 0;
	while (a != 0)
	{
		b += a % 10;
		a /= 10;
	}
	cout << b;
}
