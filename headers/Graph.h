#ifndef GRAPH_H
#define GRAPH_H

#include "List.h"
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits.h>

#define INF INT_MAX

using namespace std;

enum Colour {WHITE, GRAY, BLACK};

bool printPath_util(size_t idx, size_t j, int** paths, size_t start, size_t end);
void printPath(size_t idx, int** paths, size_t dist[], size_t n);
void printShortestPath_fixedNode(size_t idx, int** paths, size_t** dist, size_t n);
void printShortestPath(int** paths, size_t** dist, size_t n, bool fixedSource);
size_t minKey(std::vector<size_t> key, vector<bool> included_vertices, size_t V);

class AdjList{
public:

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

	// number = size_t (number of lists to initialise with)
	// directed = boolean (whether graph is directed or not)
	// weighted = boolean (whether it is weighted or not)
	AdjList(size_t number, bool _directed, bool _weighted) // CONSTRUCTOR
	{
		first_list = nullptr;
		list_counter = 0;
		vertices = 0;
		edges = 0;

		directed = _directed;
		weighted = _weighted;

		for (size_t i = 0; i < number; i++)
			addList();
	}

	// inserts list (adds vertex) into adjacency list
	bool addList()
	{
		if (first_list == nullptr)
		{
			// cout << "Added list with index %d\n", adjlist->list_counter);
			first_list = new List(list_counter);
			vertices++;
			list_counter++;
			return true;
		}

		List* listptr = first_list;

		while (listptr->next != nullptr)
			listptr = listptr->next;

		// cout << "Added list with index %d\n", adjlist->list_counter);
		listptr->next = new List(list_counter);
		
		list_counter++;
		vertices++;
		return true;
	}

	/// search for list with passed index, so we can find adjacent nodes
	List* findListWithIndex (size_t index)
	{
		List* listptr = first_list;
		while (listptr && listptr->list_index != index)
			listptr = listptr->next;
		if (listptr == nullptr)
		{
			cout << "This shouldn't happen!\n\n";
			return nullptr;
		}
		return listptr;
	}

	// removes list (removes vertex) from adjacency list
	// also needs to check all lists for references to that vertex and remove those too
	bool removeList(size_t index_to_remove)
	{
		if (first_list == nullptr)
			return false;

		List* listptr = first_list;
		List* prevlist = nullptr;
		while (listptr != nullptr)
		{
			// only removes if vertex with that index found
			listptr->remove(index_to_remove);
			
			// if this entire list has to be removed
			if (listptr->list_index == index_to_remove)
			{
				// remove all nodes in the list
				while (listptr->isEmpty() == false)
				{
					ListNode* ptr = listptr->head;
					listptr->remove(ptr->index);

					ptr = ptr->next;		
				}
				// now remove the list itself

				// if the first list was the list to remove
				if (prevlist == nullptr)
				{
					List* temp = first_list;
					first_list = first_list->next;
					delete(temp);
				}
				else
				{
					List* temp = listptr;
					prevlist->next = listptr->next;
					delete(temp);
				}
			}

			prevlist = listptr;
			listptr = listptr->next;
		}

		vertices--;
		return true;
	}

	// adds directed edge (u -> v) if adjlist->directed is true, else undirected
	// REQUIRES NODES WITH INDICES u and v TO EXIST ALREADY
	// takes in optional weight parameter (only considered if adjlist->weighted is true)
	void addEdge(size_t u, size_t v, size_t weight = 1)
	{
		if (u == v)
		{
			cout << "Can't add edge (" << u << ", " << v << ") because can't have an edge to self!\n";
			return;
		}

		List* list_u = findListWithIndex(u);
		List* list_v = findListWithIndex(v);

		if (list_u == nullptr || list_v == nullptr)
		{
			cout << "One or both of the node indices provided don't exist!";
			return;	
		}

		if (weight < 1 && weighted)
		{
			cout << "Invalid weight passed to AdjList::addEdge(), so assuming weight 1\n";
			weight = 1;
		}
		else if (weighted == false && weight != 1)
		{
			// cout << "Weight passed to AdjList::addEdge() is more than 1 but graph is non-weighted, so assuming weight 1.\n";
			weight = 1;
		}

		// insert index v in list u 
		cout << "Inserting edge (" << u << ", " << v << ") with weight " << weight << endl;

		list_u->insert(v, weight);

		// if undirected, insert index u in list v also
		if (directed == false)
			list_v->insert(u, weight);

		edges++;
	}

	// Remove specified edge (u, v) from the AdjList
	// if graph is undirected, need to remove edge (v, u) also
	void removeEdge(size_t u, size_t v)
	{
		List* listptr = findListWithIndex(u);
		if (listptr == nullptr) 
		{
			cout << "Edge with specified indices not found !!";
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
			listptr->remove(v);
			edges--;
		}
		else
		{
			cout << "Edge with specified indices not found !!";
			return;
		}

		if (directed == false)
		{
			List* listptr = findListWithIndex(v);
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
				listptr->remove(u);
				edges--;
			}
			else
			{
				// directed graph but node u not found in v's adjacency list => some problem
				cout << "UNEXPECTED ERROR IN AdjList::removeEdge()\n";
				return;
			}
		}
	}

	// utility function for depth-first TRAVERSAL (DISPLAY)
	void DF_util_show(size_t index, vector<bool> &visited)
	{
		visited[index] = true;
		cout << index << " ";

		/// search for list with passed index, so we can find adjacent nodes
		List* listptr = findListWithIndex(index);
		if (listptr == nullptr) { return; }

		/// check all nodes in listptr and push to queue if not visited
		ListNode* ptr = listptr->head;
		while(ptr != nullptr)
		{
			if (visited[ptr->index] == false)
				DF_util_show(ptr->index, visited);
			ptr = ptr->next;
		}
	}

	// Depth-first traversal DISPLAY for the graph
	void showDFT(size_t ind, bool including_disconnected_nodes)
	{
		cout << "DFT: ";
		vector<bool> visited(vertices);
		for (size_t i = 0; i < vertices; i++)
			visited[i] = false;

		if(including_disconnected_nodes)
		{
			for (size_t i = 0; i < vertices; i++)
				if (visited[i] == false)
					DF_util_show(i, visited);
		}
		else
			DF_util_show(ind, visited);
		
		cout << "\n";
	}

	// utility function for depth-first SEARCH by index
	bool DF_util_search(size_t index, size_t search_index, vector<bool> &visited) 
	{
		visited[index] = true;

		/// search for list with passed index, so we can find adjacent nodes
		List* listptr = findListWithIndex(index);
		if (listptr == nullptr) { return false; }

		/// check all nodes in listptr and push to queue if not visited
		ListNode* ptr = listptr->head;
		while(ptr != nullptr)
		{
			if (visited[ptr->index] == false)
				if(DF_util_search(ptr->index, search_index, visited))
					return true;
			ptr = ptr->next;
		}

		return false; // if not found in any list return false
	}

	// Depth-first traversal SEARCH for the graph
	// ind parameter is useless if including_disconnected_nodes is TRUE
	bool searchDFT(size_t ind, size_t search_index, bool including_disconnected_nodes)
	{
		cout << "DFT: ";
		
		vector<bool> visited(vertices);
		for (size_t i = 0; i < vertices; i++)
			visited[i] = false;

		if(including_disconnected_nodes)
		{
			for (size_t i = 0; i < vertices; i++)
				if (visited[i] == false)
					if(DF_util_search(i, search_index, visited))
						return true;

			return false;
		}
		else
			return DF_util_search(ind, search_index, visited);
		
		// cout << endl; this is never executed anyway since it's after the return
	}

	// Breadth-first traversal SEARCH for the graph
	bool searchBFT(size_t u)
	{
		cout << "\nBFT: ";

		vector<bool> visited(vertices);
		for (size_t i = 0; i < vertices; i++)
			visited[i] = false;

		// make a queue
		// Queue* queue = Queue_init();
		std::queue<size_t> que;

		List* listptr = findListWithIndex(u);
		if (listptr == nullptr) { cout << "Invalid starting node for BFS!"; return false; }
		
		visited[u] = true;
		que.push(u);

		while(que.empty() == false)
		{
			// pop from queue and check if index we are searching for is found
			size_t index = que.back(); que.pop();  // TODO: THIS WAS ORIGINALLY INT BUT CHECK IF SIZE_T WORKS AS EXPECTED
			if (index == u)
				return true; 

			/// get list whose index = index
			List* i__listptr = findListWithIndex(index);
			if (listptr->isEmpty() && listptr->next) 
			{ 
				i__listptr = listptr->next;
				listptr = listptr->next;
			}

			/// check all nodes in listptr and push to queue if not visited
			ListNode* ptr = i__listptr->head;
			if (visited[i__listptr->list_index] == false)
			{
				que.push(i__listptr->list_index);
				visited[i__listptr->list_index] = true;
			}
			while(ptr != nullptr)
			{
				if (visited[ptr->index] == false)
				{
					que.push(ptr->index);
					visited[ptr->index] = true;
				}
				ptr = ptr->next;
			}
		}

		return false; // if not found anywhere in the BFT
	}

	// used for connectivity checking
	void DFT_util_generalpurpose(size_t index, vector<bool> &visited) // RETURNED BOOL EARLIER, but I don't see the point of bool as returned value was never used
	{
		visited[index] = true;

		/// search for list with passed index, so we can find adjacent nodes
		List* listptr = findListWithIndex(index);
		if (listptr == nullptr) { return; }// false; }

		/// check all nodes in listptr and push to queue if not visited
		ListNode* ptr = listptr->head;
		while(ptr != nullptr)
		{
			if (visited[ptr->index] == false)
				DFT_util_generalpurpose(ptr->index, visited);
			ptr = ptr->next;
		}
	}

	// RETURNS WHETHER GRAPH IS CONNECTED OR NOT BY CALLING UTILITY FUNCTIONS
	bool isConnected()
	{
		// call util_checkConnectivity_DFT for index 0 and get the visited[] array
		vector<bool> visited = util_checkConnectivity_DFT(0);

		// if any node is left unvisited, that means graph is disconnected
		for(size_t i = 0; i < vertices; i++)
			if (visited[i] == false)
				return false;
		
		// if all visited, graph is connected
		return true;
	}

	// utility function that uses DFT, USED FOR CHECKING GRAPH CONNECTIVITY
	vector<bool> util_checkConnectivity_DFT(size_t ind)
	{
		vector<bool> visited(vertices);
		for (size_t i = 0; i < vertices; i++)
			visited[i] = false;

		// just call once for passed index
		DFT_util_generalpurpose(ind, visited);
		
		return visited;
	}

	// utility function that uses DFT, called by AdjList::isCyclic()
	bool util_checkCylic_DFT(size_t u, vector<int> &colour)
	{
		colour[u] = GRAY;

		/// search for list with passed index, so we can find adjacent nodes
		List* listptr = findListWithIndex(u);
		if (listptr == nullptr) { return false; }

		/// check all nodes in listptr and push to queue if not visited
		ListNode* ptr = listptr->head;
		while(ptr != nullptr)
		{
			if(colour[ptr->index] == GRAY)
				return true;
			
			if((colour[ptr->index] == WHITE) && util_checkCylic_DFT(ptr->index, colour))
				return true;

			ptr = ptr->next;
		}

		colour[u] = BLACK;

		return false;
	}

	// returns whether the graph contains a cycle or not
	bool isCyclic()
	{
		// cout << "Inside AdjList_isCyclic!\n");
		vector<int> colour(vertices);

		for (size_t i = 0; i < vertices; i++)
			colour[i] = WHITE;

		for (size_t i = 0; i < vertices; i++)
			if (colour[i] == WHITE && util_checkCylic_DFT(i, colour))
				return true;
		
		return false;
	}

	// Breadth-first traversal display for the graph
	void showBFT(size_t u)
	{
		cout << "\nBFT: ";

		vector<bool> visited(vertices);

		// make a queue
		std::queue<size_t> que;
		
		List* listptr = findListWithIndex(u);
		if (listptr == nullptr) { cout << "Invalid starting node for BFS!"; return; }
		
		visited[u] = true;
		que.push(u);
		
		while(que.empty() == false)
		{
			// pop from queue and display
			size_t index = que.front(); que.pop();		// TODO: THIS WAS ORIGINALLY INT BUT CHECK IF SIZE_T WORKS AS EXPECTED

			// if (index != -1)	// commented this out for now as size_t cannot be compared to negative number -1
			cout << index << " ";

			/// get list whose index = index
			List* i__listptr = findListWithIndex(index);

			/// if current i__listptr is empty, no path to go from here
			if (i__listptr->isEmpty())
				continue;

			/// check all nodes in listptr and push to queue if not visited
			ListNode* ptr = i__listptr->head;
			if (visited[i__listptr->list_index] == false)
			{
				// cout << "%d wasn't in queue! adding...\n", i__listptr->list_index);
				que.push(i__listptr->list_index);
				visited[i__listptr->list_index] = true;
			}

			while(ptr != nullptr)
			{
				if (visited[ptr->index] == false)
				{
					que.push(ptr->index);
					visited[ptr->index] = true;
				}				
				ptr = ptr->next;
			}
		}
		cout << "\n";
	}

	void util_TopologicalSort(size_t v, vector<bool> &visited, std::stack<size_t> &stac)
	{
		visited[v] = true;
		List* listptr = findListWithIndex(v);
		if (listptr == nullptr) { cout << "listptr is null in util_TopologicalSort!\n"; return; }
		
		ListNode* ptr = listptr->head;
		while (ptr != nullptr)
		{
			if (visited[ptr->index] == false)
			{
				// cout << "util_TopologicalSort on index %d\n", ptr->index);
				util_TopologicalSort(ptr->index, visited, stac);
			}
			ptr = ptr->next;
		}

		stac.push(v);
	}

	// Topological sort traversal display for the graph
	void TopologicalSort()
	{
		if (directed == false)
		{
			cout << "Can't do topological sort on non-directed graph!";
			return;
		}

		if (isCyclic())
		{
			cout << "Graph contains cycle, can't do topological sort!";
			return;
		}
		
		vector<bool> visited(vertices);
		for (size_t i = 0; i < vertices; i++)
			visited[i] = false;

		std::stack<size_t> stac;

		// cout << "No of vertices: %d\n", adjlist->vertices);
		for (size_t i = 0; i < vertices; i++)
			if (visited[i] == false)
				util_TopologicalSort(i, visited, stac);
		
		// cout << "Stack: "); Stack_show(stack); cout << "\n");
		cout << "Topological Sort: ";
		while(stac.empty() == false)
		{
			cout << stac.top() << " ";
			stac.pop();
		}
		cout << "\n\n";
	}

	// calculates shortest path for all pairs using Floyd-Warshall algorithm
	void FloydWarshall (bool fixedSource)
	{
		size_t** dist =  new size_t*[vertices];
		for (size_t i = 0; i < vertices; i++)
			dist[i] =  new size_t[vertices];  

		int** paths = new int*[vertices]; 
		for (size_t i = 0; i < vertices; i++)
			paths[i] =  new int[vertices];

		for(size_t i = 0; i < vertices; i++)
			for (size_t j = 0; j < vertices; j++)
			{
				dist[i][j] = INF;
				paths[i][j] = INF;
			}

		size_t i = 0, j = 0;
		List* listptr = first_list;
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

		for (size_t k = 0; k < vertices; k++)
			for (size_t i = 0; i < vertices; i++)
				for (size_t j = 0; j < vertices; j++)
				{
					if (dist[i][k] != INF && dist[k][j] != INF
					&& (dist[i][k] + dist[k][j]) < dist[i][j])
					{
						dist[i][j] = dist[i][k] + dist[k][j];
						paths[i][j] = k;
					}
				}
		for (size_t i = 0; i < vertices; i++)
			for (size_t j = 0; j < vertices; j++)
				if ((dist[i][j] != INF) && (paths[i][j] == INF))
					paths[i][j] = -1;

		/* printing Dist and Paths matrices for debug purposes
		// cout << "Dist matrix:\n");	
		// for (size_t i = 0; i < vertices; i++) 
		// { 
		//     for (size_t j = 0; j < vertices; j++) 
		//     {
		//         if (dist[i][j] == INF) 
		//             cout << "%7s", "INF"; 
		//         else
		//             cout << "%7d", dist[i][j]; 
		//     }
		//     cout << "\n"; 
		// } 

		// cout << "Paths matrix:\n";
		// for (size_t i = 0; i < vertices; i++) 
		// { 
		//     for (size_t j = 0; j < vertices; j++) 
		//     {
		//         if (paths[i][j] == INF) 
		//             cout << "%7s", "INF"; 
		//         else
		//             cout << "%7d", paths[i][j]; 
		//     }
		//     cout << "\n"; 
		// } 
		*/
		
		cout << "Actual paths with shortest distances"
			 << " between every pair of vertices \n";
		printShortestPath(paths, dist, vertices, fixedSource);
	}

	// utility function to print the constructed MST stored in parent[] 
	void printMST(vector<int> parent) 
	{ 
		cout << "Edge \tWeight\n"; 
		List* listptr = first_list;
		
		for (size_t i = 0; i < vertices; i++) 
		{
			List* listptr = findListWithIndex(i);
			ListNode* ptr = listptr->head;
			while (ptr != nullptr)
			{
				if (ptr->index == parent[i])
					printf("%d - %zu \t%zu \n", parent[i], i, ptr->weight);
				
				ptr = ptr->next;
			}
		}
	}
	
	// Constructs and prints MST for the graph
	void Prim_MST() 
	{
		if (isConnected() == false)
		{
			cout << "Graph needs to be connected for it to have a minimum spanning tree!";
			return;
		}
		if (directed)
		{
			cout << "Graph needs to be undirected for it to have a minimum spanning tree!";
			return;
		}

		// To store constructed MST
		vector<int> parent(vertices); 
		
		// Key values used to pick minimum weight edge 
		vector<size_t> key(vertices);

		// To represent set of vertices included in MST
		vector<bool> included_vertices(vertices);

		// Initialize all keys as INF 
		for (int i = 0; i < vertices; i++) 
		{
			key[i] = INF;
			included_vertices[i] = false; 
		}
	
		// Make key 0 so that first vertex is picked in the first iteration (since all others are INF) 
		key[0] = 0; 

		// since first node has no parent
		parent[0] = -1; 
	
		// The MST will have V vertices 
		List* listptr = first_list;
		if (listptr)
		while (listptr->next != nullptr) 
		{ 
			// Pick the minimum key vertex from the 
			// set of vertices not yet included in MST 
			size_t u = minKey(key, included_vertices, vertices); 

			included_vertices[u] = true; 

			// Update key value and parent index of all vertices adjacent to the picked vertex
			ListNode* ptr = listptr->head;
			while(ptr != nullptr)
			{
				// Update the key only if ptr->weight is smaller than key[v] and v is not in the List
				if (included_vertices[ptr->index] == false && ptr->weight < key[ptr->index])
					parent[ptr->index] = (int)u, key[ptr->index] = ptr->weight; 

				ptr = ptr->next;
			}
			listptr = listptr->next;
		} 

		/* just displaying parent array and included_vertices array for debug purposes

		// cout << "parent array:\n");
		// for(int i=0;i<adjlist->vertices;i++)
		// 	cout << "%d ", parent[i]);
		// cout << "\n");

		// cout << "included_vertices array:\n");
		// for(int i=0;i<adjlist->vertices;i++)
		// 	cout << "%d ", included_vertices[i]);
		// cout << "\n");
		*/
		
		printMST(parent); 
	} 

	void Util_printAllPathsForGivenSourceAndDest (size_t u, size_t v, vector<bool> &visited, vector<size_t> &paths, size_t &path_index)
	{
		visited[u] = true; 
		paths[path_index] = u; 
		(path_index)++; 

		if (u == v) 
		{ 
			for (size_t i = 0; i < (path_index); i++) 
				cout << paths[i] << " "; 
			cout << "\n";
		} 
		else // If current vertex is not destination 
		{ 
			// Recur for all the vertices adjacent to current vertex 
			List* listptr = findListWithIndex(u);
			ListNode* ptr = listptr->head;
			while (ptr != nullptr)
			{
				if (!visited[ptr->index]) 
					Util_printAllPathsForGivenSourceAndDest(ptr->index, v, visited, paths, path_index); 
				ptr = ptr->next;
			} 
		}

		// Remove current vertex from path[] and mark it as unvisited 
		(path_index)--; 
		visited[u] = false; 
	}

	// print all paths for a given source and destination
	void printAllPaths (size_t u, size_t v)
	{
		vector<bool> visited(vertices);

		for (size_t i = 0; i < vertices; i++)
			visited[i] = false;

		vector<size_t> paths(vertices);
		size_t path_index = 0;

		Util_printAllPathsForGivenSourceAndDest(u, v, visited, paths, path_index);
	}

	void show ()
	{
		cout << "\nAdjacency list: \n";
		List* listptr = first_list;
		while (listptr != nullptr)
		{
			cout << listptr->list_index << " -> ";
			listptr->show();
			cout << "\n";
			listptr = listptr->next;
		}
		cout << "\n";
	}

};

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
				cout << idx << " ";
		}
		if (printPath_util(k, j, paths, start, end))
		{
			cout << k << " ";
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
				printf("From node %zu to node %zu (path cost = %zu): %zu %zu", idx, j, dist[j], idx, j);
			else
			{
				printf("From node %zu to node %zu (path cost = %zu): %zu ", idx, j, dist[j], idx);
				printPath_util(idx, j, paths, idx, j);
				cout << j;
			}

			cout << "\n";
		}
	}
}

void printShortestPath_fixedNode(size_t idx, int** paths, size_t** dist, size_t n)
{
	printPath(idx, paths, dist[idx], n);
	cout << "\n";
}

// if fixedSource is true, prints APSP with fixed source vertex
void printShortestPath(int** paths, size_t** dist, size_t n, bool fixedSource)
{
	int x;
	if (fixedSource)
	{
		cout << "Find shortest path with which vertex as source? [enter an index]: ";
		cin >> x;

		if (x >= 0 && x < n)
			printShortestPath_fixedNode(x, paths, dist, n);
		else
			cout << "Enter a valid index greater or equal to zero, and lesser than " << n << " !!";
		return;
	}

	for (size_t i = 0; i < n; i++) 
    { 
		printShortestPath_fixedNode(i, paths, dist, n);
    }
}

// util function that finds vertex with minimum key value, from included_vertices set 
size_t minKey(std::vector<size_t> key, vector<bool> included_vertices, size_t V) 
{ 
    // Initialize min value 
    size_t min = INF, min_index;
  
    for (int v = 0; v < V; v++) 
        if (included_vertices[v] == false && key[v] < min) 
            min = key[v], min_index = v; 
  
    return min_index; 
}

#endif