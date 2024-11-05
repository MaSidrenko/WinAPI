#include<iostream>

using namespace std;

int main()
{
	int n;
	int counter = 0;
	int k = 0;
	cin >> n;
	int* a = new int[n];
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}
	cout << *max_element(a, a + n) << endl;
	for (int i = 0; i <= n; i++)
	{
		if(a[i] <= a[i + 1])
		{ 
		//cout << *find(a, a + n, *min_element(a, a + n)) << "\t";
		k++;
		}
		
	}
	for (int i = 0; i < n; i++)
	{
		while (a[i] < *max_element(a, a + n))
		{
			a[i] += 1;
		}
	}
	//for (int i = 0; i < n; i++)
	//{
	//	cout << a[i] << "\t";
	//}
	cout << endl;
	cout << k;
	delete[] a;
}
