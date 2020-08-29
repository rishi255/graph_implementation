#include "List.h"
#include "Graph.h"
#include <iostream>

using namespace std;

void menu()
{
	AdjList* adjlist = nullptr;

    int n, subchoice, init_choice;
	char ch;
	string str;

	cout <<	"\nWelcome! \nSelect a preset for initialising the empty graph:\n"
		 << "1. Directed and weighted\n"
		 << "2. Non-directed and weighted\n"
		 << "3. Directed and non-weighted\n"
		 << "4. Non-directed and non-weighted\n"
		 << "Enter a choice: ";
    cin >> init_choice;
	
	switch (init_choice)
	{
	case 1:
		adjlist = new AdjList(0, true, true);
		break;
	case 2:
		adjlist = new AdjList(0, false, true);
		break;
	case 3:
		adjlist = new AdjList(0, true, false);
		break;
	case 4:
		adjlist = new AdjList(0, false, false);
		break;
	default:
		cout <<	"\nWrong choice entered! Enter a valid choice next time.\n";
		exit(0);
	}

	// UNCOMMENT THESE TO GET A BASIC GRAPH BY DEFAULT WHEN RUNNING PROGRAM
	for (int i = 0; i < 5; i++)
		adjlist->addList();
	
	adjlist->addEdge(0, 1, 2);
	adjlist->addEdge(1, 2, 3);
	adjlist->addEdge(0, 3, 6);
	adjlist->addEdge(3, 1, 8);
	adjlist->addEdge(3, 4, 9);
	adjlist->addEdge(2, 4, 7);
	adjlist->addEdge(1, 4, 5);

	do
    {
		size_t V = adjlist->vertices;
		size_t E = adjlist->edges;

        cout << "\n * * * * * * * * * * * MENU * * * * * * * * * * * \n\n"
           	 << "1. Node operations: Add nodes, Delete nodes \n"
			 << "2. Edge operations: Add edges, Delete edges\n"
			 << "3. Display the graph via DFT, BFT or Topological Sort\n"
			 << "4. Check whether graph is connected\n"
		 	 << "5. Shortest path between a fixed source to all other vertices\n"
			 << "6. Shortest path between any pair of nodes\n"
			 << "7. Searching a value in the graph via BFS and DFS\n"
			 << "8. Detect presence of a cycle in the graph\n"
			 << "9. Calculate and display the minimum spanning tree\n"
			 << "10. Display all possible paths between a pair of nodes\n"
			 << "11. Show the adjacency list\n"
			 << "Enter your choice: ";
        cin >> n;
        cout << "\n";

        switch (n)
        {
        case 1:	// node operations
			cout <<	"1. Add a node\n"
			   	 << "2. Delete a node\n"
				 << "Enter your choice: ";
			cin >> subchoice;

			if (subchoice == 1)
			{
				int no_of_nodes;
				cout << "Current number of nodes in the graph = " << V << "\nHow many nodes do you want to add? : ";
				cin >> no_of_nodes;

				for (int i = 0; i < no_of_nodes; i++)
					adjlist->addList();
			}
			else if (subchoice == 2)
			{
				int node_to_remove;
				cout << "Enter node index to remove: ";
				cin >> node_to_remove;

				adjlist->removeList(node_to_remove);
			}
			break;

		case 2: // edge operations
			cout <<	"1. Add one or more edges\n"
			   	 <<	"2. Delete an edge\n"
				 <<	"Enter your choice: ";
			cin >> subchoice;
			if (subchoice == 1)
			{
				int no_of_edges;
				cout << "\nHow many edges do you want to add? : ";
				cin >> no_of_edges;

				for (int i = 0; i < no_of_edges; i++)
				{
					size_t u, v;
					cout << "\nEnter start and end of edge: ";
					cin >> u >> v;

					if (adjlist->weighted)
					{
						size_t weight;
						cout << "Enter weight of edge (" << u << ", " << v << "): ";
						cin >> weight;

						adjlist->addEdge(u, v, weight);
					}
					else
						adjlist->addEdge(u, v, 1);
				}
			}
			else if (subchoice == 2)
			{
				size_t u, v;
				cout << "\nEnter start and end of edge to remove: ";
				cin >> u >> v;

				adjlist->removeEdge(u, v);
			}
			break;
		
		case 3: // BFT, DFT and Topological Sort operations
			cout <<	"1. BFT display of graph\n"
			   	 <<	"2. DFT display of graph\n"
				 << "3. Topological Sort\n"
				 << "Enter your choice: ";
			cin >> subchoice;

			if (subchoice == 1)
			{
				size_t u;
				cout << "Enter starting node for BFT traversal: ";
				cin >> u;
				adjlist->showBFT(u);
			}
			else if (subchoice == 2)
			{
				int u;
				cout << "Enter starting node for DFT traversal (enter -1 to loop all nodes & display disconnected nodes too): ";
				cin >> u;

				if (u == -1)
					adjlist->showDFT(0, true);
				else
					adjlist->showDFT(u, false);
			}
			else if (subchoice == 3)
			{
				adjlist->TopologicalSort();
			}
			break;
		
		case 4:
			adjlist->isConnected() ? cout << "\nGraph is connected!" : cout << "\nGraph is disconnected!";
			break;
		
		case 5:
			adjlist->FloydWarshall(true);
			break;

		case 6:
			adjlist->FloydWarshall(false);
			break;

		case 7:
			cout << "1. Search using BFS\n"
				 << "2. Search using DFS (excluding disconnected nodes)\n"
				 << "Enter your choice: ";
			cin >> subchoice;

			size_t u;
			cout << "Enter value to search for: ";
			cin >> u;

			if (subchoice == 1)
				adjlist->searchBFT(u);
			else
				adjlist->searchDFT(0, u, false);
			break;

		case 8:
			adjlist->isCyclic() ? cout << "\nThe graph contains a cycle!" : cout << "\nThe graph does not contain a cycle!";
			break;
		
		case 9:
			adjlist->Prim_MST();
			break;

		case 10:
		{
			size_t u, v;
			cout << "\nEnter start and end nodes: ";
			cin >> u >> v;
			adjlist->printAllPaths(u, v);
			break;
		}
		case 11:
			adjlist->show();
			break;

        default: 
            break;
        }
        cout << "\nDo you want to continue? [y/n]: ";
        cin >> str;
        cout << "\n\n";
        ch = str[0];
    }while(ch=='y' || ch=='Y');
}

int main()
{
	menu();
	return 0;
}

/* commented out method calls for some reason
	// non-directed and weighted
	// AdjList* adjlist = AdjList_init(5, false, true);
	// AdjList_show(adjlist);

	// AdjList_addEdge(adjlist, 0, 1, 2);
	// AdjList_addEdge(adjlist, 1, 2, 3);
	// AdjList_addEdge(adjlist, 0, 3, 6);
	// AdjList_addEdge(adjlist, 3, 1, 8);
	// AdjList_addEdge(adjlist, 3, 4, 9);
	// AdjList_addEdge(adjlist, 2, 4, 7);
	// AdjList_addEdge(adjlist, 1, 4, 5);

	// AdjList_removeList(adjlist, 0);
	// AdjList_removeEdge(adjlist, 1, 3);

	/// ALL SHOW METHODS:

	// AdjList_show(adjlist);
	// AdjList_showBFT(adjlist, 5);
	// AdjList_showDFT(adjlist, 5, false);
	// AdjList_TopologicalSort(adjlist);
	// AdjList_FloydWarshall(adjlist, false);
	// AdjList_Prim_MST(adjlist);
	// AdjList_printAllPaths(adjlist, 0, 4);
*/