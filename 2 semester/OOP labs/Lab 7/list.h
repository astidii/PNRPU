#pragma once
#include <iostream>
using namespace std;

template <typename T>
struct Node
{
	Node<T>* next = nullptr;
	Node<T>* prev = nullptr;
	T data;
};


class  Pair {
	int x;
	double y;

public:
	Pair() {
		this->x = 0;
		this->y = 0;
	}

	Pair(int x, double y) {
		this->x = x;
		this->y = y;
	}

	Pair(const Pair& other) {
		this->x = other.x;
		this->y = other.y;
	}

	~Pair() {}

	Pair operator=(const Pair& p) {
		this->x = p.x;
		this->y = p.y;
		return *this;
	}

	friend ostream& operator<<(ostream& stream, const Pair& p) {
		stream << '(' << p.x << ":" << p.y << ')' << " ";
		return stream;
	}

	friend istream& operator>>(istream& stream, Pair& p) {
		cout << "\nInput x: ";
		stream >> p.x;
		cout << "Input y: ";
		stream >> p.y;
		return stream;
	}
};

template <class T>
class List
{
private:
	Node<T>* head;
	Node<T>* tail;
	int size;

public:
	List() { head = nullptr, tail = nullptr, size = 0; }
	~List() { Clear(); }


	void push_back(T element)
	{
		Node<T>* new_node = new Node<T>;
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

		size++;
	}

	void push_front(T element)
	{
		Node<T>* new_node = new Node<T>;
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

		size++;
	}


	void pop_back()
	{
		if (head == nullptr)
		{
			cout << "List is empty!";
			return;
		}

		Node<T>* temp_node;

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

	}


	void pop_front()
	{
		if (head == nullptr)
		{
			cout << "\nList is empty!";
			return;
		}

		Node<T>* temp_node;

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

	}


	void Clear()
	{
		while (head != nullptr)
		{
			Node<T>* temp_node = head;
			head = head->next;
			delete temp_node;
		}

		size = 0;
	}

	template <typename T>
	friend ostream& operator<<(ostream& out, const List<T>& list);

	template <typename T>
	friend istream& operator>>(istream& in, List<T>& list);

	T& operator[](int index)
	{
		/*if (index < 0 || index >= size)
		{
			return
		}*/

		Node<T>* current_node = head;

		for (int i = 0; i < index; i++)
		{
			current_node = current_node->next;
		}

		return current_node->data;
	}


	List operator*(const List<T>& otherList)
	{
		List res;

		Node<T>* current_node1 = head;
		Node<T>* current_node2 = otherList.head;

		while (current_node1 != nullptr && current_node2 != nullptr)
		{
			res.push_back(current_node1->data * current_node2->data);
			current_node1 = current_node1->next;
			current_node2 = current_node2->next;
		}

		return res;
	}

};

template <typename T>
ostream& operator<<(ostream& out, const List<T>& list)
{
	if (list.size == 0)
	{
		cout << "\nList is empty!";
	}

	else
	{
		Node<T>* current_node = list.head;
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


template <typename T>
istream& operator>>(istream& in, List<T>& list)
{
	list.Clear();

	int size;

	cout << "List size: ";
	in >> size;

	for (int i = 0; i < size; i++)
	{
		T data;
		cout << "Input element " << i + 1 << ": ";
		in >> data;
		list.push_back(data);
	}

	return in;
}
