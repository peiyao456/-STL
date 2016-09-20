#pragma once
#include<iostream>
using namespace std;
#include"TypeTraits.h"
#include<memory.h>

template<typename T>
class Vector
{
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;
	Vector()
		:_start(NULL)
		,_finish(NULL)
		,_EndOfStorage(NULL)
	{}
	~Vector()
	{}
	int Size()
	{
		return _finish - _start;
	}
	void PushBack(const T& x)
	{
		CheckStorage();
		*_finish = x;
		_finish++;
	}
	void PopBack()
	{
		Erase(--_finish);
	}
	void PushFront(const T& x)
	{
		Insert(_start,x);
	}
	void PopFront()
	{
		Erase(_start);
		--_finish;
	}
	void Erase(Iterator Position)//删除pos位置的元素
	{
		if (Position + 1 != End())
		{
			for (Iterator pos = Position + 1; pos < _finish; ++pos)
			{
				*(pos - 1) = *pos;
			}
		}
	}
	void Insert(Iterator Position, const T& x)
	{
		CheckStorage();
		for (Iterator pos = End() - 1;pos >= Position; --pos)
		{
			*(pos + 1) = *pos;
		}
		*_start = x;
		++_finish;
	}
	Iterator Begin()
	{
		//return _start;
		return Iterator(_start);
	}
	Iterator End()
	{
		return _finish;
	}
	ConstIterator Begin() const
	{
		return _start;
	}
	ConstIterator End() const
	{
		return _finish;
	}
	T& operator*()
	{
		return *this;
	}
protected:
	void CheckStorage()
	{
		if (_finish == _EndOfStorage)
		{
			int size = Size();
			int NewSize =2*  size + 3;
			T* tmp = new T[NewSize];
			if (TypeTraits<T>::__IsPODType().Get())
			{
				memcpy(tmp,_start,Size()*sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < Size(); ++i)
				{
					tmp[i] = _start[i];
				}
			}
			delete _start;
			_start = tmp;
			_finish = _start + size;
			_EndOfStorage = _start + NewSize;
		}
	}
private:
	Iterator _start;
	Iterator _finish;//指向最后一个元素的下一个位置
	Iterator _EndOfStorage;
};

void TestVector()
{
	Vector<int> v1;
	v1.PushBack(1);
	v1.PushBack(2);
	v1.PushBack(3);
	v1.PushBack(4);
	v1.PushBack(5);
	v1.PopBack();

	v1.PushFront(6);
	v1.PopFront();

	Vector<int>::ConstIterator it = v1.Begin();
	//Vector<int>::Iterator it = v1.Begin();
	while (it < v1.End())
	{
		//*it = 9;
		cout << *it << endl;
		++it;
	}

}