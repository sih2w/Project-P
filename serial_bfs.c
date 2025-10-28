#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define target 1999999
#define start 0

typedef struct {
    int total_vertices;
    int edge_count;
    int *row_ptr;
    int *col_index;
} CSR;

CSR* load_graph(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    CSR *g = malloc(sizeof(CSR));
    if (!g) { fclose(f); return NULL; }

    // read counts
    fread(&g->total_vertices, sizeof(int), 1, f);
    fread(&g->edge_count, sizeof(int), 1, f);

    // allocate arrays
    g->row_ptr = malloc((g->total_vertices + 1) * sizeof(int));
	g->col_index = malloc(g->edge_count * sizeof(int));
	
    if (!g->row_ptr || !g->col_index) {
        fclose(f);
        free(g->row_ptr); free(g->col_index); free(g);
        return NULL;
    }

    // read arrays
    fread(g->row_ptr,   sizeof(int), g->total_vertices + 1, f);
    fread(g->col_index, sizeof(int), g->edge_count, f);

    fclose(f);
    return g;
}

void free_graph(CSR *g) {
    if (!g) return;
    free(g->row_ptr);
    free(g->col_index);
    free(g);
}

void busy_work() {
    for(int i = 0; i < 20000; i++) {
		int tmp = 1;
	}
	return;
}

int main() {

	// Load info and store in graph struct
	CSR *graph = load_graph("savefile");

	////////////////// Testing output ///////////////////////
	// printf("\nrow_ptr:\n");
	// // Print for testing
	// for(int i = 0; i < graph->total_vertices; i++) {
	// 	printf("%d ", graph->row_ptr[i]);
	// }
	// printf("\ncol_index:\n");

	// // Print for testing
	// for(int i = 0; i < graph->edge_count; i++) {
	// 	printf("%d ", graph->col_index[i]);
	// }
	// printf("\n\n");

	////////////////// Serial Search ///////////////////////

	// Initialize Queue and Visited Arrays
	int *visited = calloc(graph->total_vertices, sizeof(int));
    int *queue = malloc(graph->total_vertices * sizeof(int));

	// Track front and rear of queue
    int front = 0;
	int end = 0;
	int found = 0;

	// enqueue starting node
	queue[end++] = start;

	visited[start] = 1;
	int nodes_checked = 0;

	
	// Nodes are checked in edge-pairs. u is connected to v by an edge.
	while(front < end) {
		// dequeue next node
		int u = queue[front++];
		nodes_checked++;

		// Do busy work
		busy_work();

		// Look through all of its neigbors
		for(int i = graph->row_ptr[u]; i < graph->row_ptr[u + 1]; i++) {
			// Get a neigbor
			int v = graph->col_index[i];

			// Check if its the target
			if(v == target) {
				printf("Found!\n");
				front = end;
				found = 1;
				break;
			}
			// If neighbor hasnt been visited, queue it up for further search
			if(!visited[v]) {
				visited[v] = 1;
				queue[end++] = v;
			}
		}
	}

	if(!found)
		printf("Not Found!\n");
	
	printf("Nodes Checked: %d\n", nodes_checked);
	
	free(visited);
	free(queue);
	free_graph(graph);
	
	return 0;
}




























