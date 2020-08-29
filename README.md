# Graph Implementation in C++

An implementation of the graph data structure in C++, done using adjacency lists.
This was originally done entirely in C, then ported to C++ manually. It uses the modern C++ STL library.  
   
## How to build and run:  

This project uses [CMake](https://cmake.org/), so if you don't have it, you'll need to get it from [here](https://cmake.org/download/) before proceeding.  
You can also install it through [Cygwin](https://www.cygwin.com/) (Windows), [MacPorts](https://www.macports.org/) (MacOS), [Homebrew](https://brew.sh/) (MacOS/Linux) or [MSYS2](https://www.msys2.org/) (Windows).

Once CMake is installed:
1. Open a terminal.  
2. `cd` to the directory where you want to download the repo  
3. `git clone https://github.com/rishi255/graph_implementation.git`
4. Once done, follow these steps in the command line to build (works on PowerShell too):  
   * `cd graph_implementation`  
   * `cmake -B build`
   * `cmake --build build --config Release`
5. To run the project:  
   - `cd build/Release`
   - `./Graph_Implementation.exe` (for Windows) or `./Graph_Implementation` (for Unix)
   
## Features: 

This project is an implementation of the Graph data structure in C using adjacency lists that can perform the following operations on a graph:

1. **Node operations:** Add a node, Delete a node
2. **Edge operations:** Add an edge, Delete an edge
3. **Displaying the graph** via **Depth-First Traversal**, **Breadth-First Traversal** and **Topological Sort**
4. **Detect** whether the graph is **connected** or **disconnected**.
5. **Shortest path** between a **fixed source** to all other vertices (can calculate path as well as path-cost)
6. **Shortest path** between **any pair of nodes**. (can calculate path as well as path-cost)
7. **Searching a value** in the graph via **breadth-first search (BFS)** and **depth-first search (DFS)**
8. Detect the **presence of a cycle in the graph**
9. Calculate and **display the minimum spanning tree** (uses Prim’s algorithm for MST)
10. **Display all possible paths** between a pair of nodes

This program is interactive and has a user-friendly menu that lets the user select a choice, and displays useful error information if invalid inputs are entered, and also prints information regarding the request made in certain other situations (like adding a node).

It can operate on weighted/non-weighted and directed/undirected graphs, according to what the user selects.
