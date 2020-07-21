#include "List.h"
#include "Graph.h"
#include <stdio.h>

void menu()
{
	AdjList* adjlist = nullptr;

	char str[10], ch;
    int n, subchoice, init_choice;

	printf(	"\nWelcome! \nSelect a preset for initialising the empty graph:\n"
			"1. Directed and weighted\n"
			"2. Non-directed and weighted\n"
			"3. Directed and non-weighted\n"
			"4. Non-directed and non-weighted\n"
			"Enter a choice: ");
    scanf("%d", &init_choice);
	switch (init_choice)
	{
	case 1:
		adjlist = AdjList_init(0, true, true);
		break;
	case 2:
		adjlist = AdjList_init(0, false, true);
		break;
	case 3:
		adjlist = AdjList_init(0, true, false);
		break;
	case 4:
		adjlist = AdjList_init(0, false, false);
		break;
	default:
		printf(	"\nGRAPH INITIALIZATION FAILED! Enter a valid choice next time.\n");
		exit(0);
	}

	// UNCOMMENT THESE TO GET A BASIC GRAPH BY DEFAULT WHEN RUNNING PROGRAM
	for (int i = 0; i < 5; i++)
		AdjList_addList(adjlist);
	
	AdjList_addEdge(adjlist, 0, 1, 2);
	AdjList_addEdge(adjlist, 1, 2, 3);
	AdjList_addEdge(adjlist, 0, 3, 6);
	AdjList_addEdge(adjlist, 3, 1, 8);
	AdjList_addEdge(adjlist, 3, 4, 9);
	AdjList_addEdge(adjlist, 2, 4, 7);
	AdjList_addEdge(adjlist, 1, 4, 5);

	do
    {
		size_t V = adjlist->vertices;
		size_t E = adjlist->edges;

        printf("\n * * * * * * * * * * * MENU * * * * * * * * * * * \n\n");
        printf(	"1. Node operations: Add nodes, Delete nodes \n"
			   	"2. Edge operations: Add edges, Delete edges\n"
			   	"3. Display the graph via DFT, BFT or Topological Sort\n"
			   	"4. Check whether graph is connected\n"
		 	   	"5. Shortest path between a fixed source to all other vertices\n"
			   	"6. Shortest path between any pair of nodes\n"
				"7. Searching a value in the graph via BFS and DFS\n"
				"8. Detect presence of a cycle in the graph\n"
				"9. Calculate and display the minimum spanning tree\n"
				"10. Display all possible paths between a pair of nodes\n"
				"11. Show the adjacency list\n"
				"Enter your choice: ");
        scanf("%d", &n);
        printf("\n");
        switch (n)
        {
        case 1:	// node operations
			printf(	"1. Add a node\n"
			   		"2. Delete a node\n"
					"Enter your choice: ");
			scanf("%d", &subchoice);
			if (subchoice == 1)
			{
				int no_of_nodes;
				printf("Current number of nodes in the graph = %d\nHow many nodes do you want to add? : ", V);
				scanf("%d", &no_of_nodes);

				for (int i = 0; i < no_of_nodes; i++)
					AdjList_addList(adjlist);
			}
			else if (subchoice == 2)
			{
				int node_to_remove;
				printf("Enter node index to remove: ");
				scanf("%d", &node_to_remove);

				AdjList_removeList(adjlist, node_to_remove);
			}
			break;

		case 2: // edge operations
			printf(	"1. Add one or more edges\n"
			   		"2. Delete an edge\n"
					"Enter your choice: ");
			scanf("%d", &subchoice);
			if (subchoice == 1)
			{
				int no_of_edges;
				printf("\nHow many edges do you want to add? : ");
				scanf("%d", &no_of_edges);

				for (int i = 0; i < no_of_edges; i++)
				{
					size_t u, v;
					printf("\nEnter start and end of edge: ");
					scanf("%d%d", &u, &v);
					if (adjlist->weighted)
					{
						size_t weight;
						printf("Enter weight of edge (%d, %d): ", u, v);
						scanf("%d",&weight);
						AdjList_addEdge(adjlist, u, v, weight);
					}
					else
						AdjList_addEdge(adjlist, u, v, 1);
				}
			}
			else if (subchoice == 2)
			{
				size_t u, v;
				printf("\nEnter start and end of edge to remove: ");
				scanf("%d%d", &u, &v);

				AdjList_removeEdge(adjlist, u, v);
			}
			break;
		
		case 3: // BFT, DFT and Topological Sort operations
			printf(	"1. BFT display of graph\n"
			   		"2. DFT display of graph\n"
					"3. Topological Sort\n"
					"Enter your choice: ");
			scanf("%d", &subchoice);
			if (subchoice == 1)
			{
				size_t u;
				printf("Enter starting node for BFT traversal: ");
				scanf("%d", &u);
				AdjList_showBFT(adjlist, u);
			}
			else if (subchoice == 2)
			{
				int u;
				printf("Enter starting node for DFT traversal (enter -1 to loop all nodes & display disconnected nodes too): ");
				scanf("%d", &u);

				if (u == -1)
					AdjList_showDFT(adjlist, 0, true);
				else
					AdjList_showDFT(adjlist, u, false);
			}
			else if (subchoice == 3)
			{
				AdjList_TopologicalSort(adjlist);
			}
			break;
		
		case 4:
			AdjList_isConnected(adjlist) ? printf("\nGraph is connected!") : printf("Graph is disconnected!");
			break;
		
		case 5:
			AdjList_FloydWarshall(adjlist, true);
			break;

		case 6:
			AdjList_FloydWarshall(adjlist, false);
			break;

		case 7:
			printf(	"1. Search using BFS\n"
					"2. Search using DFS (excluding disconnected nodes)\n"
					"Enter your choice: ");
			scanf("%d", &subchoice);

			size_t u;
			printf("Enter value to search for: ");
			scanf("%d", &u);
			if (subchoice == 1)
				AdjList_searchBFT(adjlist, u);
			else
				AdjList_searchDFT(adjlist, 0, u, false);
			break;

		case 8:
			AdjList_isCyclic(adjlist) ? printf("\nThe graph contains a cycle!") : printf("\nThe graph does not contain a cycle!");
			break;
		
		case 9:
			AdjList_Prim_MST(adjlist);
			break;

		case 10:
		{
			size_t u, v;
			printf("\nEnter start and end nodes: ");
			scanf("%d%d", &u, &v);
			AdjList_printAllPaths(adjlist, u, v);
			break;
		}
		case 11:
			AdjList_show(adjlist);
			break;

        default: 
            break;
        }
        printf("\nDo you want to continue? [y/n]: ");
        scanf("%s", &str);
        printf("\n\n");
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