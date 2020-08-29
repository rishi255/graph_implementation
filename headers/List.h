#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// single node of adjacency list of ith node
class ListNode
{
public:
	int data;

	// weight of path from ith node to this node
	// (default weight = 1)
	size_t weight;

	// index of vertex
	size_t index;

	ListNode* next;

	// ListNode constructor with optional weight (from ith node to this node)
	// default weight = 1
	ListNode(size_t _index, size_t _weight = 1)
	{
		index = _index;
		next = nullptr;
		weight = _weight;
	}
	
};

// a single list, n such lists will form adjacency list
class List
{
public:
	ListNode* head;
	size_t list_index;
	size_t list_size;

	List* next;

	// List constructor (need to provide index (vertex number))
	List(size_t _index)
	{
		head = nullptr;
		next = nullptr;

		list_size = 0;
		list_index = _index;
	}

	// returns whether list is empty
	bool isEmpty() { return (head == nullptr); }

	// searches for a vertex number in the list
	// returns pointer to node if found, else returns nullptr
	ListNode* find(size_t index)
	{
		if(isEmpty())
			return nullptr;

		ListNode* ptr = head;
		while(ptr != nullptr)
		{
			if (ptr->index == index)
				return ptr;

			ptr = ptr->next;
		}
		return nullptr;
	}

	// insert into list, increment list size (default weight = 1)
	bool insert(size_t idx, size_t _weight = 1)
	{
		if (isEmpty())
		{
			head = new ListNode(idx, _weight); //;_init(idx);
			list_size++;

			return true;
		}

		ListNode* ptr = head;

		while (ptr->next != nullptr)// && ptr->index < )
			ptr = ptr->next;
		
		ptr->next = new ListNode(idx, _weight);

		list_size++;
		return true;
	}

	// calls find and searches for a vertex (whose index is the index passed)
	// if found, remove vertex from list and decrement list size
	bool remove(size_t idx)
	{
		if (find(idx) == nullptr)
			return false;
		
		if (head->index == idx)
		{
			ListNode* temp = head;
			head = head->next;
			delete(temp);

			list_size--;
			return true;
		}

		ListNode* ptr = head;

		ListNode* prevptr = nullptr;
		while (ptr != nullptr)
		{
			if (ptr->index == idx && prevptr)
			{
				ListNode* temp = ptr;
				prevptr->next = ptr->next;
				delete(temp);

				list_size--;
				return true;
			}
			prevptr = ptr;
			ptr = ptr->next;
		}

		return false;
	}

	// pop from front of list
	bool pop_front()
	{
		if(isEmpty())
			return false;
		
		ListNode* temp = head;
		head = head->next;
		delete(temp);

		list_size--;
		return true;
	}

	// show list
	void show()
	{
		if (isEmpty())
		{
			printf("[empty]");
			return;
		}

		ListNode* ptr = head;
		while (ptr != nullptr)
		{
			printf("%zu ", ptr->index); // , ptr->data
			if (ptr->next)
				printf(", ");
			ptr = ptr->next;
		}
	}

};

#endif