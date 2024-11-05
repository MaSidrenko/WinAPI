#include<iostream>

using namespace std;

int NOD(int a, int b)
{
	while (a > 0 && b > 0)

		if (a > b)
			a %= b;

		else
			b %= a;

	return a + b;
}

void test(int n)
{
	for (int i = 2; i <= n; i++)
	{
		if (i * i > n) i = n;
		while (n % i == 0)
		{
			cout << i << " ";
			n /= i;
		}
	}
}

int main()
{
	int simple_num;
	int task, a ,b;
	cin >> task;
	for (int i = 0; i < task; i++)
	{
		a = 0; b = 0;
		cin >> a >> b;
		
	int c = NOD(a , b);
		for (int i = 2; i <= c; i++)
		{
			if (i * i > c) i = c;
			while (c % i == 0)
			{
				cout << i << " ";
				c /= i;
			}
		}
	
	}
}