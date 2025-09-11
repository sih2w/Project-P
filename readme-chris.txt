- To run each .c file, just type "make" then "run_" where _ is the first letter of the file. This will auto recompile if changes were made.

- If you want to change vertex and edge count, edit the makefile. The run graph line shows the run command in order:
./"object" "vertices" "edges" "seed" "directed flag"

This is the current pipeline:
1. Run graph.c to generate the graph. This graph is currently stored in a .txt called "edgelist.txt".
2. Run compress.c to flatten the list of edges to two arrays. This file also stores the arrays in a binary file called "savefile".
3. Run load.c to extract the graph from the binary and print the arrays that were in there. I couldn't print them in the pretty Vertex - {} format because im missing an array for it, but compress.c does that and explains how the arrays work.
