#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define nullptr NULL

// single node of adjacency list of ith node
typedef struct ListNode
{
	int data;

	// weight of path from ith node to this node
	// (default weight = 1)
	size_t weight;

	// index of vertex
	size_t index;

	struct ListNode* next;
}ListNode;

// ListNode constructor with no weight provided
// default weight = 1
ListNode* ListNode_init(size_t index)
{
	ListNode* node = (ListNode*) malloc(sizeof(*node));
	node->index = index;
	node->next = nullptr;
	node->weight = 1;
	return node;
}

// ListNode constructor with weight (from ith node to this node) provided
ListNode* ListNode_init_weighted(size_t index, size_t weight)
{
	ListNode* node = (ListNode*) malloc(sizeof(*node));
	node->index = index;
	node->next = nullptr;
	node->weight = weight;
	return node;
}


// a single list, n such lists will form adjacency list
typedef struct List
{
	ListNode* head;
	size_t list_index;
	size_t list_size;

	struct List* next;
}List;

// List constructor (need to provide index (vertex number))
List* List_init(size_t index)
{
	List* list = (List*) malloc(sizeof(*list));
	
	list->head = nullptr;
	list->next = nullptr;

	list->list_size = 0;
	list->list_index = index;
	
	return list;
}

bool List_isEmpty(List* list)
{
	return (list->head == nullptr);
}

// searches for a vertex number in the list
// returns pointer to node if found, else returns nullptr
ListNode* List_find(List* list, size_t index)
{
	if(List_isEmpty(list))
		return nullptr;

	ListNode* ptr = list->head;
	while(ptr != nullptr)
	{
		if (ptr->index == index)
			return ptr;

		ptr = ptr->next;
	}
	return nullptr;
}

// insert into list, increment list size
bool List_insert (List* list, size_t idx)
{
	if (List_isEmpty(list))
	{
		list->head = ListNode_init(idx);

		list->list_size++;
		return true;
	}

	ListNode* ptr = list->head;

	while (ptr->next != nullptr)// && ptr->index < )
		ptr = ptr->next;
	
	ptr->next = ListNode_init(idx);

	list->list_size++;
	return true;
}

// ONLY CALLED IF ADJLIST IS WEIGHTED
// insert into list, increment list size
bool List_insert_weighted (List* list, size_t idx, size_t weight)
{
	if (List_isEmpty(list))
	{
		list->head = ListNode_init_weighted(idx, weight);

		list->list_size++;
		return true;
	}

	ListNode* ptr = list->head;

	while (ptr->next != nullptr)// && ptr->index < )
		ptr = ptr->next;
	
	ptr->next = ListNode_init_weighted(idx, weight);

	list->list_size++;
	return true;
}

// calls find and searches for a vertex (whose index is the index passed)
// if found, remove vertex from list and decrement list size
bool List_remove (List* list, size_t index)
{
	if (List_find(list, index) == nullptr)
		return false;
	
	if (list->head->index == index)
	{
		ListNode* temp = list->head;
		list->head = list->head->next;
		free(temp);

		list->list_size--;
		return true;
	}

	ListNode* ptr = list->head;

	ListNode* prevptr = nullptr;
	while (ptr != nullptr)
	{
		if (ptr->index == index && prevptr)
		{
			ListNode* temp = ptr;
			prevptr->next = ptr->next;
			free(temp);

			list->list_size--;
			return true;
		}
		prevptr = ptr;
		ptr = ptr->next;
	}

	return false;
}

// pop from front of list
bool List_pop_front (List* list)
{
	if(List_isEmpty(list))
		return false;
	
	ListNode* temp = list->head;
	list->head = list->head->next;
	free(temp);

	list->list_size--;
	return true;
}

// show list
void List_show(List* list)
{
	if (List_isEmpty(list))
	{
		printf("[empty]");
		return;
	}

	ListNode* ptr = list->head;
	while (ptr != nullptr)
	{
		printf("%d ", ptr->index); // , ptr->data
		if (ptr->next)
			printf(", ");
		ptr = ptr->next;
	}
}

typedef struct QueueNode
{
	size_t data;
	struct QueueNode* next;
}QueueNode;

QueueNode* QueueNode_init(size_t x)
{
	QueueNode* node = (QueueNode*) malloc(sizeof(*node));
	node->data = x;
	node->next = nullptr;

	return node;
}

typedef struct Queue
{
	QueueNode* head;
	size_t queue_size;
}Queue;

Queue* Queue_init()
{
	Queue* queue = (Queue*) malloc(sizeof(*queue));
	queue->head = nullptr;
	queue->queue_size = 0;

	return queue;
}

bool Queue_isEmpty(Queue* queue)
{
	return (queue->head == nullptr);
}

// push to back of queue, increment size
bool Queue_push (Queue* queue, size_t data)
{
	if (Queue_isEmpty(queue))
	{
		queue->head = QueueNode_init(data);

		queue->queue_size++;
		return true;
	}

	QueueNode* ptr = queue->head;
	while (ptr->next != nullptr)
		ptr = ptr->next;
	ptr->next = QueueNode_init(data);

	queue->queue_size++;
	return true;
}

// remove node from front of queue
int Queue_pop(Queue* queue)
{
	if (Queue_isEmpty(queue))
		return -1;
	
	QueueNode* temp = queue->head;
	int ind = temp->data;
	queue->head = queue->head->next;
	free(temp);

	queue->queue_size--;
	return ind;
}

// show queue 
void Queue_show(Queue* queue)
{
	if (Queue_isEmpty(queue))
	{
		printf("[empty]");
		return;
	}

	QueueNode* ptr = queue->head;
	while (ptr != nullptr)
	{
		printf("%d ", ptr->data);
		if (ptr->next)
			printf(", ");
		ptr = ptr->next;
	}
}

typedef struct Stack
{
	QueueNode* head;
}Stack;

Stack* Stack_init()
{
	Stack* stack = (Stack*) malloc(sizeof(*stack));
	stack->head = nullptr;

	return stack;
}

bool Stack_isEmpty(Stack* stack)
{
	return (stack->head == nullptr);
}

// push to back of stack
bool Stack_push (Stack* stack, size_t data)
{
	if (Stack_isEmpty(stack))
	{
		stack->head = QueueNode_init(data);
		return true;
	}

	QueueNode* ptr = stack->head;
	while (ptr->next != nullptr)
		ptr = ptr->next;
	ptr->next = QueueNode_init(data);

	return true;
}

// remove node from top of stack
int Stack_pop(Stack* stack)
{
	if (Stack_isEmpty(stack))
		return -1;
	QueueNode* prev = nullptr;
	QueueNode* temp = stack->head;
	// reach last node
	while(temp->next != nullptr)
	{
		prev = temp;
		temp = temp->next;
	}

	// if only one element was in stack
	if (prev == nullptr)
	{
		int d = stack->head->data;
		free(stack->head);
		stack->head = nullptr;
		return d;
	}
	
	int ind = temp->data;
	prev->next = temp->next;
	free(temp);
	return ind;
}

// show Stack 
void Stack_show(Stack* stack)
{
	if (Stack_isEmpty(stack))
	{
		printf("[empty]");
		return;
	}

	QueueNode* ptr = stack->head;
	while (ptr != nullptr)
	{
		printf("%d", ptr->data);
		if (ptr->next)
			printf(", ");
		ptr = ptr->next;
	}
}

#endif