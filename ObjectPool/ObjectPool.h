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
		size_t _objNum;//�ڴ����ĸ���

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
	size_t _countIn;//��ǰ������õļ���
	BlockNode* _first;
	BlockNode* _last;
	size_t _maxNum;
	static size_t _itemSize;//��������Ĵ�С
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

		//����һ���µĽ��ռ�
		_last->_next = new BlockNode(newObjectNum);
		//����ǰ���õļ�����Ϊ0
		_countIn = 0;
		_last = _last->_next;
	}

	
	T* New()
	{
		//����ʹ�����һ���ͷŵĿռ�
		if(_lastDelete)
		{
			T* obj = _lastDelete;
			_lastDelete = *((T**)_lastDelete);
			return new (obj)T;
		}

		//���ڴ�����������
		//���н����ڴ�鶼û�п���ʹ�õģ������¿����µĿռ�
		if(_last->_objNum == _countIn)//���һ�������ڴ�Ҳ��������
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


//���Դ��룺���Բ��Զ�������ͷ�