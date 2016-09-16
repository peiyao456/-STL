#pragma once

#include<iostream>
using namespace std;
#include<cassert>

template<typename T>
struct __LinkNode//链表结点
{
	__LinkNode<T>* _next;
	__LinkNode<T>* _prev;
	T _data;
	__LinkNode(T x)
		:_data(x)
		,_next(NULL)
		,_prev(NULL)
	{}
};
template<typename T,typename Ref,typename Ptr>
struct __List_Iterator//链表迭代器
{
	typedef __LinkNode<T> Node;
	typedef __List_Iterator<T, Ref, Ptr> Self;
	typedef __List_Iterator<T, T&, T*>  Iterator;

	__List_Iterator(Node* node)
		:_node(node)//带参的构造函数
	{}

	__List_Iterator()//不带参的构造函数
	{}

	__List_Iterator(const Iterator &  x)//拷贝构造函数
		:_node(x._node)
	{}

	bool operator== (const Self& x)const
	{
		return x._node == _node;
	}

	bool operator!=(const Self& x)const
	{
		return x._node != _node;
	}
	Ref operator*()const
	{
		return (*_node)._data;
	}

	Self& operator++()
	{
		_node = (*_node)._next;
		return *this;
	}
	Self operator++(int)
	{
		Self tmp(*this);
		++*tmp;
		return tmp;
	}

	Self& operator--()
	{
		_node = (*_node)._prev;
		return *this;
	}
	Self operator--(int)
	{
		Self tmp(*this);
		--*tmp;
		return tmp;
	}
	Node* _node;
};

template<typename T>
class List
{
public:
	typedef __LinkNode<T> Node;
	typedef __List_Iterator<T, T&, T*>  Iterator;
	List()
	{
		_head = BuyNode(T());
		_head->_prev = _head;
		_head->_next = _head;
	}
	Node* BuyNode(const T& x)
	{
		Node* node = new Node(x);
		return node;
	}
	void PushBack(const T& x)
	{
		Node* tmp = BuyNode(x);
		Node* tail = _head->_prev;

		tail->_next = tmp;
		tmp->_prev = tail;

		tmp->_next = _head;
		_head->_prev = tmp;
	}
	void Insert(Iterator pos, const T& x)
	{
		Node* tmp = BuyNode(x);
		Node* cur = pos._node;
		Node* prev = cur->_prev;
		
		//插入元素
		tmp->_next = cur;
		cur->_prev = tmp;

		prev->_next = tmp;
		tmp->_prev = prev;
	

		//插入位置在第一个结点之前
		if (prev->_next == _head)
		{
			_head = tmp;
		}
	}
	Iterator Begin()
	{
		return Iterator(_head->_next);
	}
	Iterator End()
	{
		return Iterator(_head);
	}
	void Erase(Iterator pos)
	{
		assert(pos != End());
		Node* del = pos._node;
		Node* next = del->_next;
		Node* prev = del->_prev;

		next->_prev = prev;
		prev->_next = next;

		//处理删除第一个结点的情况
		if (del == _head)
		{
			_head = next;
		}

		delete del;
	}
	void PushFront(const T& x)
	{
		Insert(Begin(),x);
	}
	void PopBack()
	{
		Erase(Iterator(_head->_prev));
	}
	void PopFront()
	{
		Erase(Iterator(_head->_next));
	}
	void Display()
	{
		if (_head->_next != NULL)
		{
			Node* cur = _head->_next;
			while (cur->_next != _head)
			{
				cout << cur->_data << endl;
				cur = cur->_next;
			}
			cout << cur->_data << endl;
		}
	}
protected:
	Node* _head;
};
