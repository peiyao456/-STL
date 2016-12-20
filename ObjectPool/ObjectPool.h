#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<string>

template<typename T>
class ObjectPool
{
	struct BlockNode
	{
		void* _memory;
		BlockNode* _next;
		size_t _objNum;//内存对象的个数

		BlockNode(size_t objNum)
			:_objNum(objNum)
			,_next(NULL)
		{
			_memory = malloc(_itemSize * _objNum);
			if(_memory == NULL)
				throw -1;
		}
		~BlockNode()
		{
			free(_memory);
			_memory = NULL;
			_objNum = 0;
		}
	};
protected:
	size_t _countIn;//当前结点在用的计数
	BlockNode* _first;
	BlockNode* _last;
	size_t _maxNum;
	static size_t _itemSize;//单个对象的大小
	T* _lastDelete;
public:
	ObjectPool(size_t initNum = 32,size_t maxNum = 10000)
	{
		_countIn = 0;
		_maxNum = maxNum;
		_first = _last = new BlockNode(initNum);
		_lastDelete = NULL;
	}

	~ObjectPool()
	{
		BlockNode* cur = _first;
		while(cur)
		{
			BlockNode* del = cur;
			cur = cur->_next;
			delete del;
		}
		_first = _last = NULL;
		_countIn = 0;
	}

	static size_t GetItemSize()
	{
		if(sizeof(T) < sizeof(T*))
			return sizeof(T*);
		else
		{
			return sizeof(T);
		}
	}

	void Allocate()
	{
		size_t newObjectNum = _last->_objNum * 2;
		if(newObjectNum >= _maxNum)
				newObjectNum = _maxNum;

		//开辟一个新的结点空间
		_last->_next = new BlockNode(newObjectNum);
		//将当前在用的计数改为0
		_countIn = 0;
		_last = _last->_next;
	}

	
	T* New()
	{
		//优先使用最后一次释放的空间
		if(_lastDelete)
		{
			T* obj = _lastDelete;
			_lastDelete = *((T**)_lastDelete);
			return new (obj)T;
		}

		//从内存块里进行申请
		//所有结点的内存块都没有可以使用的，则重新开辟新的空间
		if(_last->_objNum == _countIn)//最后一个结点的内存也被用完了
		{
			Allocate();
		}
		
		T* mem = (T*)((char*)(_last->_memory) + _countIn * _itemSize);
		_countIn++;
		return new(mem) T;

	}
	void Delete(T* ptr)
	{
		if(ptr)
		{
			ptr->~T();
			
		}
		*(T**) ptr = _lastDelete;
		_lastDelete = ptr;
	}
};

template<typename T>
size_t ObjectPool<T>::_itemSize = ObjectPool<T>::GetItemSize();


//测试代码：可以测试多次申请释放