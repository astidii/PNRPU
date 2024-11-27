#pragma once
#include <iostream>
using namespace std;

struct Node
{
	Node* next = nullptr;
	Node* prev = nullptr;
	int data;
};

class Iterator
{
	friend class List;

private:
	Node* elem;

public:
	Iterator() { elem = nullptr;}
	Iterator(const Iterator& it) { elem = it.elem; }
	bool operator==(const Iterator& it) { return elem == it.elem; }
	bool operator!=(const Iterator& it) { return elem != it.elem; }
	int& operator*() { return elem->data; }
	Iterator& operator++()
	{
		if (elem != nullptr)
		{
			elem = elem->next;
		}
		return *this;
	}

	Iterator& operator--()
	{
		if (elem != nullptr)
		{
			elem = elem->prev;
		}
		return *this;
	}

	Iterator& operator+(int n)
	{
		Iterator it = *this;
		for (int i = 0; i < n && it.elem != nullptr; i++)
		{
			it.elem = it.elem->next;
		}
		return it;
	}

	
};


class List
{
private:
	Node* head;
	Node* tail;
	int size;
	Iterator beg;
	Iterator end;

public:
	List() { head = nullptr, tail = nullptr, size = 0; }
	~List() { Clear(); }
	Iterator first() { return beg; }
	Iterator last() { return end; }

	void push_back(int element)
	{
		Node* new_node = new Node;
		new_node->data = element;

		if (head == nullptr)
		{
			head = new_node;
			tail = new_node;
		}

		else
		{
			tail->next = new_node;
			new_node->prev = tail;
			tail = new_node;
		}

		beg.elem = head;
		end.elem = tail;

		size++;
	}

	void push_front(int element)
	{
		Node* new_node = new Node;
		new_node->data = element;

		if (head == nullptr)
		{
			head = new_node;
			tail = new_node;
		}

		else
		{
			new_node->next = head;
			head->prev = new_node;
			head = new_node;
		}

		beg.elem = head;
		end.elem = tail;

		size++;
	}


	void pop_back()
	{
		if (head == nullptr)
		{
			cout << "List is empty!";
			return;
		}

		Node* temp_node;

		if (head == tail)
		{
			temp_node = head;

			head = nullptr;
			tail = nullptr;
		}

		else
		{
			temp_node = tail;
			tail = tail->prev;
			tail->next = nullptr;
		}

		delete temp_node;
		size--;
		beg.elem = head;
		end.elem = tail;
	}


	void pop_front()
	{
		if (head == nullptr)
		{
			cout << "\nList is empty!";
			return;
		}

		Node* temp_node;

		if (head == tail)
		{
			temp_node = head;

			head = nullptr;
			tail = nullptr;
		}

		else
		{
			temp_node = head;
			head = head->next;
			head->prev = nullptr;
		}

		delete temp_node;
		size--;
		beg.elem = head;
		end.elem = tail;
	}


	void Clear()
	{
		while (head != nullptr)
		{
			Node* temp_node = head;
			head = head->next;
			delete temp_node;
		}

		size = 0;
	}


	friend ostream& operator<<(ostream&out, const List&list);
	friend istream& operator>>(istream&in, List&list);

	int& operator[](int index)
	{

		Node* current_node = head;

		for (int i = 0; i < index; i++)
		{
			current_node = current_node ->next;
		}

		return current_node ->data;
	}

	List operator*(const List& otherList)
	{
		List res;

		Node* current_node1 = head;
		Node* current_node2 = otherList.head;

		while (current_node1 != nullptr && current_node2 != nullptr)
		{
			res.push_back(current_node1->data * current_node2->data);
			current_node1 = current_node1->next;
			current_node2 = current_node2->next;
		}

		return res;
	}
	
};


ostream& operator<<(ostream& out, const List& list)
{
	if (list.size == 0)
	{
		cout << "\nList is empty!";
	}

	else
	{
		Node* current_node = list.head;
		out << current_node->data;
		current_node = current_node->next;

		while (current_node != nullptr)
		{
			out << " " << current_node->data;
			current_node = current_node->next;
		}
	}

	cout << endl;

	return out;
}


istream& operator>>(istream& in, List& list)
{
	list.Clear();

	int size;

	cout << "List size: "; 
	in >> size;

	for (int i = 0; i < size; i++)
	{
		int data;
		cout << "Input element " << i + 1 << ": ";
		in >> data;
		list.push_back(data);
	}

	return in;
}
