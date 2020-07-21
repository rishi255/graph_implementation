#ifndef GRAPH_H
#define GRAPH_H

#include "List.h"
#include <stdio.h>
#include <limits.h>

#define nullptr NULL
#define INF INT_MAX

typedef enum Colour {WHITE, GRAY, BLACK} Colour;

typedef struct AdjList{
	List* first_list;

	// just a counter to know what index number to give newly made vertices
	size_t list_counter;
	
	// holds total number of lists currently in adjacency list, or no of vertices in graph
	// (vertices <= list counter)
	size_t vertices;
	
	// no of edges
	size_t edges;

	// whether directed or not
	bool directed;

	// whether weighted graph or not
	bool weighted;
}AdjList;

bool AdjList_addList(AdjList* adjlist);

// number = size_t (number of lists to initialise with)
// directed = boolean (whether graph is directed or not)
// weighted = boolean (whether it is weighted or not)
AdjList* AdjList_init(size_t number, bool directed, bool weighted)
{
	AdjList* adjlist = (AdjList*) malloc(sizeof(*adjlist));

	adjlist->first_list = nullptr;
	
	adjlist->list_counter = 0;
	adjlist->vertices = 0;
	adjlist->edges = 0;
	
	adjlist->directed = directed;
	adjlist->weighted = weighted;

	for (size_t i = 0; i < number; i++)
		AdjList_addList(adjlist);
	return adjlist;
}

/// search for list with passed index, so we can find adjacent nodes
List* AdjList_findListWithIndex (AdjList* adjlist, size_t index)
{
	List* listptr = adjlist->first_list;
	while (listptr && listptr->list_index != index)
		listptr = listptr->next;
	if (listptr == nullptr)
	{
		printf("This shouldn't happen!\n\n");
		return nullptr;
	}

	return listptr;
}

// inserts list (adds vertex) into adjacency list
bool AdjList_addList(AdjList* adjlist)
{
	if (adjlist->first_list == nullptr)
	{
		// printf("Added list with index %d\n", adjlist->list_counter);
		adjlist->first_list = List_init(adjlist->list_counter);
		adjlist->vertices++;
		adjlist->list_counter++;
		return true;
	}

	List* listptr = adjlist->first_list;

	while (listptr->next != nullptr)
		listptr = listptr->next;

	// printf("Added list with index %d\n", adjlist->list_counter);
	listptr->next = List_init(adjlist->list_counter);
	
	adjlist->list_counter++;
	adjlist->vertices++;
	return true;
}

// removes list (removes vertex) from adjacency list
// also needs to check all lists for references to that vertex and remove those too
bool AdjList_removeList(AdjList* adjlist, size_t index_to_remove)
{
	if (adjlist->first_list == nullptr)
		return false;

	List* listptr = adjlist->first_list;
	List* prevlist = nullptr;
	while (listptr != nullptr)
	{
		// only removes if vertex with that index found
		List_remove(listptr, index_to_remove);
		
		// if this entire list has to be removed
		if (listptr->list_index == index_to_remove)
		{
			// remove all nodes in the list
			while (List_isEmpty(listptr) == false)
			{
				ListNode* ptr = listptr->head;
				List_remove(listptr, ptr->index);

				ptr = ptr->next;		
			}
			// now remove the list itself

			// if the first list was the list to remove
			if (prevlist == nullptr)
			{
				List* temp = adjlist->first_list;
				adjlist->first_list = adjlist->first_list->next;
				free(temp);
			}
			else
			{
				List* temp = listptr;
				prevlist->next = listptr->next;
				free(temp);
			}
		}

		prevlist = listptr;
		listptr = listptr->next;
	}

	adjlist->vertices--;
	return true;
}

// adds directed edge (u -> v) if adjlist->directed is true, else undirected
// REQUIRES NODES WITH INDICES u and v TO EXIST ALREADY
// takes in weight parameter (only considered if adjlist->weighted is true)
// Validates weights as well (can't enter weight lesser than 1 => can't enter negative weights also)
void AdjList_addEdge(AdjList* adjlist, size_t u, size_t v, size_t weight)
{
	if (u == v)
	{
		printf("Can't add edge (%d, %d) because can't have an edge to self!\n", u, v);
		return;
	}

	List* list_u = AdjList_findListWithIndex(adjlist, u);
	List* list_v = AdjList_findListWithIndex(adjlist, v);

	if (list_u == nullptr || list_v == nullptr)
	{
		printf("One or both of the node indices provided don't exist!");
		return;	
	}

	// insert index v in list u 
	if(adjlist->weighted)
	{
		if ((int)(weight) < 1)
		{
			printf("Can't add edge (%d, %d), weight should have a minimum value of 1 and can't be negative!\n", u, v);
			return;
		}
		printf("Inserting edge (%d, %d) with weight %zu\n", u, v, weight);
		List_insert_weighted(list_u, v, weight);
	}
	else
		List_insert(list_u, v);
	
	// if undirected, insert index u in list v also
	// no need to validate weight here as already done and returned if invalid
	if (adjlist->directed == false)
	{
		if(adjlist->weighted)
			List_insert_weighted(list_v, u, weight);
		else
			List_insert(list_v, u);
	}

	adjlist->edges++;
}

// Remove specified edge (u, v) from the AdjList
// if graph is undirected, need to remove edge (v, u) also
void AdjList_removeEdge(AdjList* adjlist, size_t u, size_t v)
{
	List* listptr = AdjList_findListWithIndex(adjlist, u);
	if (listptr == nullptr) 
	{
		printf("Edge with specified indices not found !!");
		return;
	}

	ListNode* ptr = listptr->head;
	bool found = false;
	while (ptr != nullptr)
	{
		if (ptr->index == v)
			found = true;
		ptr = ptr->next;
	}

	if (found)
	{
		List_remove(listptr, v);
		adjlist->edges--;
	}
	else
	{
		printf("Edge with specified indices not found !!");
		return;
	}

	if (adjlist->directed == false)
	{
		List* listptr = AdjList_findListWithIndex(adjlist, v);
		ListNode* ptr = listptr->head;
		bool found = false;
		while (ptr != nullptr)
		{
			if (ptr->index == u)
				found = true;
			ptr = ptr->next;
		}

		if (found)
		{
			List_remove(listptr, u);
			adjlist->edges--;
		}
		else
		{
			printf("UNEXPECTED ERROR IN AdjList_removeEdge()\n");
			return;
		}
	}
}

// utility function for depth-first TRAVERSAL (DISPLAY)
void DF_util_show(AdjList *adjlist, size_t index, bool visited[])
{
	visited[index] = true;
	printf("%d ", index);

	/// search for list with passed index, so we can find adjacent nodes
	List* listptr = AdjList_findListWithIndex(adjlist, index);
	if (listptr == nullptr) { return; }

	/// check all nodes in listptr and push to queue if not visited
	ListNode* ptr = listptr->head;
	while(ptr != nullptr)
	{
		if (visited[ptr->index] == false)
			DF_util_show(adjlist, ptr->index, visited);
		ptr = ptr->next;
	}
}

// Depth-first traversal DISPLAY for the graph
void AdjList_showDFT(AdjList *adjlist, size_t ind, bool including_disconnected_nodes)
{
	printf("DFT: ");
	bool *visited = (bool*) malloc(adjlist->vertices * sizeof(*visited));
	for (size_t i = 0; i < adjlist->vertices; i++)
		visited[i] = false;

	if(including_disconnected_nodes)
	{
		for (size_t i = 0; i < adjlist->vertices; i++)
			if (visited[i] == false)
				DF_util_show(adjlist, i, visited);
	}
	else
		DF_util_show(adjlist, ind, visited);
	
	printf("\n");
}

// utility function for depth-first SEARCH by index
bool DF_util_search(AdjList *adjlist, size_t index, size_t search_index, bool visited[])
{
	visited[index] = true;

	/// search for list with passed index, so we can find adjacent nodes
	List* listptr = AdjList_findListWithIndex(adjlist, index);
	if (listptr == nullptr) { return false; }

	/// check all nodes in listptr and push to queue if not visited
	ListNode* ptr = listptr->head;
	while(ptr != nullptr)
	{
		if (visited[ptr->index] == false)
			if(DF_util_search(adjlist, ptr->index, search_index, visited))
				return true;
		ptr = ptr->next;
	}
}

// Depth-first traversal SEARCH for the graph
// ind parameter is useless if including_disconnected_nodes is TRUE
bool AdjList_searchDFT(AdjList *adjlist, size_t ind, size_t search_index, bool including_disconnected_nodes)
{
	printf("DFT: ");
	bool *visited = (bool*) malloc(adjlist->vertices * sizeof(*visited));
	for (size_t i = 0; i < adjlist->vertices; i++)
		visited[i] = false;

	if(including_disconnected_nodes)
	{
		for (size_t i = 0; i < adjlist->vertices; i++)
			if (visited[i] == false)
				if(DF_util_search(adjlist, i, search_index, visited))
					return true;

		return false;
	}
	else
		return DF_util_search(adjlist, ind, search_index, visited);
	
	printf("\n");
}

// Breadth-first traversal SEARCH for the graph
bool AdjList_searchBFT(AdjList *adjlist, size_t u)
{
	printf("\nBFT: ");

	bool *visited = (bool*) malloc(adjlist->vertices * sizeof(*visited));
	for (size_t i = 0; i < adjlist->vertices; i++)
		visited[i] = false;

	// make a queue
	Queue* queue = Queue_init();
	List* listptr = AdjList_findListWithIndex(adjlist, u);
	if (listptr == nullptr) { printf("Invalid starting node for BFS!"); return false; }
	
	visited[u] = true;
	Queue_push(queue, u);

	while(Queue_isEmpty(queue) == false)
	{
		// pop from queue and check if index we are searching for is found
		int index = Queue_pop(queue);
		if (index == u)
			return true; 

		/// get list whose index = index
		List* i__listptr = AdjList_findListWithIndex(adjlist, index);
		if (List_isEmpty(listptr) && listptr->next) 
		{ 
			i__listptr = listptr->next;
			listptr = listptr->next;
		}

		/// check all nodes in listptr and push to queue if not visited
		ListNode* ptr = i__listptr->head;
		if (visited[i__listptr->list_index] == false)
		{
			Queue_push(queue, i__listptr->list_index);
			visited[i__listptr->list_index] = true;
		}
		while(ptr != nullptr)
		{
			if (visited[ptr->index] == false)
			{
				Queue_push(queue, ptr->index);
				visited[ptr->index] = true;
			}
			ptr = ptr->next;
		}
	}

	return false; // if not found anywhere in the BFT
}

// used for connectivity checking
bool DFT_util_generalpurpose(AdjList *adjlist, size_t index, bool visited[])
{
	visited[index] = true;

	/// search for list with passed index, so we can find adjacent nodes
	List* listptr = AdjList_findListWithIndex(adjlist, index);
	if (listptr == nullptr) { return false; }

	/// check all nodes in listptr and push to queue if not visited
	ListNode* ptr = listptr->head;
	while(ptr != nullptr)
	{
		if (visited[ptr->index] == false)
			DFT_util_generalpurpose(adjlist, ptr->index, visited);
		ptr = ptr->next;
	}
}

// utility function that uses DFT, USED FOR CHECKING GRAPH CONNECTIVITY
bool* util_checkConnectivity_DFT(AdjList *adjlist, size_t ind)
{
	bool *visited = (bool*) malloc(adjlist->vertices * sizeof(*visited));
	for (size_t i = 0; i < adjlist->vertices; i++)
		visited[i] = false;

	// just call once for passed index
	DFT_util_generalpurpose(adjlist, ind, visited);
	
	return visited;
}

bool AdjList_isConnected(AdjList* adjlist)
{
	// call util_checkConnectivity_DFT for index 0 and get the visited[] array
	bool* visited = util_checkConnectivity_DFT(adjlist, 0);

	// if any node is left unvisited, that means graph is disconnected
	for(size_t i = 0; i < adjlist->vertices; i++)
		if (visited[i] == false)
			return false;
	
	// if all visited, graph is connected
	return true;
}

// utility function that uses DFT, called by AdjList_isCyclic()
bool util_checkCylic_DFT(AdjList* adjlist, size_t u, int colour[])
{
	colour[u] = GRAY;

	/// search for list with passed index, so we can find adjacent nodes
	List* listptr = AdjList_findListWithIndex(adjlist, u);
	if (listptr == nullptr) { return false; }

	/// check all nodes in listptr and push to queue if not visited
	ListNode* ptr = listptr->head;
	while(ptr != nullptr)
	{
		if(colour[ptr->index] == GRAY)
			return true;
		
		if((colour[ptr->index] == WHITE) && util_checkCylic_DFT(adjlist, ptr->index, colour))
			return true;

		ptr = ptr->next;
	}

	colour[u] = BLACK;

	return false;
}

// returns whether the graph contains a cycle or not
bool AdjList_isCyclic(AdjList* adjlist)
{
	// printf("Inside AdjList_isCyclic!\n");
	int* colour = (int*) malloc(adjlist->vertices * sizeof(*colour));
	for (size_t i = 0; i < adjlist->vertices; i++)
		colour[i] = WHITE;

	for (size_t i = 0; i < adjlist->vertices; i++)
		if (colour[i] == WHITE 
			&& util_checkCylic_DFT(adjlist, i, colour))
				return true;
	return false;
}

// Breadth-first traversal display for the graph
void AdjList_showBFT(AdjList *adjlist, size_t u)
{
	printf("\nBFT: ");

	bool *visited = (bool*) malloc(adjlist->vertices * sizeof(*visited));
	for (size_t i = 0; i < adjlist->vertices; i++)
		visited[i] = false;

	// make a queue
	Queue* queue = Queue_init();
	List* listptr = AdjList_findListWithIndex(adjlist, u);
	if (listptr == nullptr) { printf("Invalid starting node for BFS!"); return; }
	
	visited[u] = true;
	Queue_push(queue, u);

	while(Queue_isEmpty(queue) == false)
	{
		// pop from queue and display
		int index = Queue_pop(queue);
		if (index != -1)
			printf("%d ", index); 

		/// get list whose index = index
		List* i__listptr = AdjList_findListWithIndex(adjlist, index);
		if (List_isEmpty(listptr) && listptr->next) 
		{ 
			i__listptr = listptr->next;
			listptr = listptr->next;
		}

		/// check all nodes in listptr and push to queue if not visited
		ListNode* ptr = i__listptr->head;
		if (visited[i__listptr->list_index] == false)
		{
			// printf("%d wasn't in queue! adding...\n", i__listptr->list_index);
			Queue_push(queue, i__listptr->list_index);
			visited[i__listptr->list_index] = true;
		}
		while(ptr != nullptr)
		{
			if (visited[ptr->index] == false)
			{
				Queue_push(queue, ptr->index);
				visited[ptr->index] = true;
			}
			ptr = ptr->next;
		}
	}
	printf("\n");
}

void util_TopologicalSort(AdjList* adjlist, int v, bool visited[], Stack* stack)
{
	visited[v] = true;
	List* listptr = AdjList_findListWithIndex(adjlist, v);
	if (listptr == nullptr) { printf("listptr is null in util_TopologicalSort!\n"); return; }
	
	ListNode* ptr = listptr->head;
	while (ptr != nullptr)
	{
		if (visited[ptr->index] == false)
		{
			// printf("util_TopologicalSort on index %d\n", ptr->index);
			util_TopologicalSort(adjlist, ptr->index, visited, stack);
		}
		ptr = ptr->next;
	}

	Stack_push(stack, v);
}

// Topological sort traversal display for the graph
void AdjList_TopologicalSort(AdjList *adjlist)
{
	if (adjlist->directed == false)
	{
		printf("Can't do topological sort on non-directed graph!");
		return;
	}

	if (AdjList_isCyclic(adjlist))
	{
		printf("Graph contains cycle, can't do topological sort!");
		return;
	}
	
	bool *visited = (bool*) malloc(adjlist->vertices * sizeof(*visited));
	for (size_t i = 0; i < adjlist->vertices; i++)
		visited[i] = false;

	Stack* stack = Stack_init();

	// printf("No of vertices: %d\n", adjlist->vertices);
	for (size_t i = 0; i < adjlist->vertices; i++)
		if (visited[i] == false)
			util_TopologicalSort(adjlist, i, visited, stack);
	
	// printf("Stack: "); Stack_show(stack); printf("\n");
	printf("Topological Sort: ");
	while(Stack_isEmpty(stack) == false)
	{
		printf("%d ", Stack_pop(stack));
	}
	printf("\n\n");
}

// bool, returns true if direct edge from idx to j, otherwise false
bool printPath_util(size_t idx, size_t j, int** paths, size_t start, size_t end)
{
	if (paths[idx][j] == -1)
		return true;

	size_t k = paths[idx][j];
	if (k != -1)
	{
		if ( printPath_util(idx, k, paths, start, end))
		{
			if (idx != start)
				printf("%d ", idx);
		}
		if (printPath_util(k, j, paths, start, end))
		{
			printf("%d ", k);
		}
	}

	return false;
}

void printPath(size_t idx, int** paths, size_t dist[], size_t n)
{
	for(size_t j = 0; j < n; j++)
	{
		if ((paths[idx][j] != INF) && (idx != j))
		{
			if (paths[idx][j] == -1)
				printf("From node %d to node %d (path cost = %d): %d %d", idx, j, dist[j], idx, j);
			else
			{
				printf("From node %d to node %d (path cost = %d): %d ", idx, j, dist[j], idx);
				printPath_util(idx, j, paths, idx, j);
				printf("%d", j);
			}

			printf("\n");
		}
	}
}

void printShortestPath_fixedNode(size_t idx, int** paths, size_t** dist, size_t n)
{
	printPath(idx, paths, dist[idx], n);
	printf("\n");
}

// if fixedSource is true, prints APSP with fixed source vertex
void printShortestPath(int** paths, size_t** dist, size_t n, bool fixedSource)
{
	int x;
	if (fixedSource)
	{
		printf("Find shortest path with which vertex as source? [enter an index]: ");
		scanf("%d", &x);
		if (x >= 0 && x < n)
			printShortestPath_fixedNode(x, paths, dist, n);
		else
			printf("Enter a valid index greater or equal to zero, and lesser than %d !!", n);
		return;
	}

	for (size_t i = 0; i < n; i++) 
    { 
		printShortestPath_fixedNode(i, paths, dist, n);
    }
}

// calculates shortest path for all pairs using Floyd-Warshall algorithm
void AdjList_FloydWarshall (AdjList* adjlist, bool fixedSource)
{
	size_t** dist = (size_t**) malloc(adjlist->vertices * sizeof(size_t*));
	for (size_t i = 0; i < adjlist->vertices; i++)
		dist[i] =  (size_t*) malloc(adjlist->vertices * sizeof(*dist[i]));

	int** paths = (int**) malloc(adjlist->vertices * sizeof(int*));
	for (size_t i = 0; i < adjlist->vertices; i++)
		paths[i] =  (int*) malloc(adjlist->vertices * sizeof(*paths[i]));

	for(size_t i = 0; i < adjlist->vertices; i++)
		for (size_t j = 0; j < adjlist->vertices; j++)
		{
			dist[i][j] = INF;
			paths[i][j] = INF;
		}

	size_t i = 0, j = 0;
	List* listptr = adjlist->first_list;
	while(listptr != nullptr)
	{
		i = listptr->list_index;

		ListNode* ptr = listptr->head;
		while(ptr != nullptr)
		{
			j = ptr->index;

			dist[i][j] = ptr->weight; // weight of path from i to j
			
			ptr = ptr->next;
		}

		listptr = listptr->next;
	}

	for (size_t k = 0; k < adjlist->vertices; k++)
		for (size_t i = 0; i < adjlist->vertices; i++)
			for (size_t j = 0; j < adjlist->vertices; j++)
			{
				if (dist[i][k] != INF && dist[k][j] != INF
				&& (dist[i][k] + dist[k][j]) < dist[i][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					paths[i][j] = k;
				}
			}
	for (size_t i = 0; i < adjlist->vertices; i++)
		for (size_t j = 0; j < adjlist->vertices; j++)
			if ((dist[i][j] != INF) && (paths[i][j] == INF))
				paths[i][j] = -1;

	/* printing Dist and Paths matrices for debug purposes
	// printf("Dist matrix:\n");	
	// for (size_t i = 0; i < adjlist->vertices; i++) 
    // { 
    //     for (size_t j = 0; j < adjlist->vertices; j++) 
    //     {
    //         if (dist[i][j] == INF) 
    //             printf("%7s", "INF"); 
    //         else
    //             printf ("%7d", dist[i][j]); 
    //     }
    //     printf("\n"); 
    // } 

	// printf("Paths matrix:\n");
    // for (size_t i = 0; i < adjlist->vertices; i++) 
    // { 
    //     for (size_t j = 0; j < adjlist->vertices; j++) 
    //     {
    //         if (paths[i][j] == INF) 
    //             printf("%7s", "INF"); 
    //         else
    //             printf ("%7d", paths[i][j]); 
    //     }
    //     printf("\n"); 
    // } 
	*/
	
	printf ("Actual paths with shortest distances"
            " between every pair of vertices \n");
	printShortestPath(paths, dist, adjlist->vertices, fixedSource);
}

// util function that finds vertex with minimum key value, from included_vertices set 
int minKey(int key[], bool included_vertices[], size_t V) 
{ 
    // Initialize min value 
    int min = INF, min_index;
  
    for (int v = 0; v < V; v++) 
        if (included_vertices[v] == false && key[v] < min) 
            min = key[v], min_index = v; 
  
    return min_index; 
} 
  
// utility function to print the constructed MST stored in parent[] 
int printMST(AdjList* adjlist, int parent[]) 
{ 
    printf("Edge \tWeight\n"); 
	List* listptr = adjlist->first_list;
	
    for (int i = 0; i < adjlist->vertices; i++) 
	{
		List* listptr = AdjList_findListWithIndex(adjlist, i);
		ListNode* ptr = listptr->head;
		while (ptr != nullptr)
		{
			if (ptr->index == parent[i])
        		printf("%d - %d \t%d \n", parent[i], i, ptr->weight);
			
			ptr = ptr->next;
		}
	}
}
  
// Constructs and prints MST for the graph
void AdjList_Prim_MST(AdjList* adjlist) 
{
	if (AdjList_isConnected(adjlist) == false)
	{
		printf("Graph needs to be connected for it to have a minimum spanning tree!");
		return;
	}
	if (adjlist->directed)
	{
		printf("Graph needs to be undirected for it to have a minimum spanning tree!");
		return;
	}

    // To store constructed MST 
	int* parent = (int*) malloc(adjlist->vertices * sizeof(int));

    // Key values used to pick minimum weight edge 
    int* key = (int*) malloc(adjlist->vertices * sizeof(int));

    // To represent set of vertices included in MST
	bool* included_vertices = (bool*) malloc(adjlist->vertices * sizeof(bool));
  
    // Initialize all keys as INF 
    for (int i = 0; i < adjlist->vertices; i++) 
	{
        key[i] = INF;
		included_vertices[i] = false; 
	}
  
    // Make key 0 so that first vertex is picked in the first iteration (since all others are INF) 
    key[0] = 0; 

	// since first node has no parent
    parent[0] = -1; 
  
    // The MST will have V vertices 
	List* listptr = adjlist->first_list;
	if (listptr)
    while (listptr->next != nullptr) 
	{ 
        // Pick the minimum key vertex from the 
        // set of vertices not yet included in MST 
        int u = minKey(key, included_vertices, adjlist->vertices); 

        included_vertices[u] = true; 

        // Update key value and parent index of all vertices adjacent to the picked vertex
		ListNode* ptr = listptr->head;
        while(ptr != nullptr)
		{
            // Update the key only if ptr->weight is smaller than key[v] and v is not in the List
            if (included_vertices[ptr->index] == false && ptr->weight < key[ptr->index])
                parent[ptr->index] = u, key[ptr->index] = ptr->weight; 

			ptr = ptr->next;
		}
		listptr = listptr->next;
    } 

	/* just displaying parent array and included_vertices array for debug purposes

	// printf("parent array:\n");
	// for(int i=0;i<adjlist->vertices;i++)
	// 	printf("%d ", parent[i]);
	// printf("\n");

	// printf("included_vertices array:\n");
	// for(int i=0;i<adjlist->vertices;i++)
	// 	printf("%d ", included_vertices[i]);
	// printf("\n");
	*/
    
	printMST(adjlist, parent); 
} 

void Util_printAllPathsForGivenSourceAndDest (AdjList* adjlist, size_t u, size_t v, bool visited[], size_t paths[], size_t *path_index)
{
	visited[u] = true; 
    paths[*path_index] = u; 
    (*path_index)++; 

	if (u == v) 
    { 
        for (size_t i = 0; i < (*path_index); i++) 
        	printf("%zu ", paths[i]); 
        printf("\n");
    } 
	else // If current vertex is not destination 
    { 
        // Recur for all the vertices adjacent to current vertex 
        List* listptr = AdjList_findListWithIndex(adjlist, u);
		ListNode* ptr = listptr->head;
        while (ptr != nullptr)
		{
            if (!visited[ptr->index]) 
                Util_printAllPathsForGivenSourceAndDest(adjlist, ptr->index, v, visited, paths, path_index); 
			ptr = ptr->next;
		} 
    }

	// Remove current vertex from path[] and mark it as unvisited 
    (*path_index)--; 
    visited[u] = false; 
}

// print all paths for a given source and destination
void AdjList_printAllPaths (AdjList* adjlist, size_t u, size_t v)
{
	bool *visited = (bool*) malloc(adjlist->vertices * sizeof(*visited));
	for (size_t i = 0; i < adjlist->vertices; i++)
		visited[i] = false;

	size_t* paths = (size_t*) malloc(adjlist->vertices * sizeof(*paths));
	size_t path_index = 0;

	Util_printAllPathsForGivenSourceAndDest(adjlist, u, v, visited, paths, &path_index);
}

void AdjList_show (AdjList* adjlist)
{
	printf("\nAdjacency list: \n");
	List* listptr = adjlist->first_list;
	while (listptr != nullptr)
	{
		printf("%d -> ", listptr->list_index);
		List_show(listptr);
		printf("\n");
		listptr = listptr->next;
	}
	printf("\n");
}

#endif