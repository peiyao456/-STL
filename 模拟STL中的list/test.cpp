#include"List.h"

void test()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);
	l.PushFront(0);
	l.PopBack();
	l.Display();
	l.PopFront();
	l.Display();
	/*List<int>::Iterator it = l.Begin();
	while (it != l.End())
	{
		cout << *it << endl;
		++it;
	}*/
}
int main()
{
	test();
	system("pause");
	return 0;
}