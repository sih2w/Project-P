#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>
#include <string.h>

#define start 0
#define THREAD_COUNT 32

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
    for(int i = 0; i < 10000; i++) {
		int tmp = 1;
	}
	return;
}

int main(int argc, char* argv[]) {

	// Load info and store in graph struct
	CSR *graph = load_graph("savefile");

	omp_set_num_threads(THREAD_COUNT);

	unsigned int seed = atoi(argv[1]);

	// Generate random target number
	srand(seed);
	int target = rand() % graph->total_vertices; 

	printf("\n\n---- Parallel ----\n");
	printf("\nTarget: %d\n", target);
	printf("Searching...\n");

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

	

	// Initialize Queue and Visited Arrays
	int *visited = calloc(graph->total_vertices, sizeof(int));

	int nodes_checked = 0;

	// Track front and rear of queue
    int front = 0;
	int end = 0;
	int found = 0;

	// Initialize Current Frontier of Nodes
	int *current_frontier = malloc(graph->total_vertices * sizeof(int));
	int *next_frontier = malloc(graph->total_vertices * sizeof(int));
	
	int current_frontier_size = 0;
	int next_size = 0;
	
	current_frontier[current_frontier_size] = start;
	current_frontier_size++;

	visited[start] = 1;

	while(current_frontier_size > 0) {
		#pragma omp parallel
		{
			int *local = malloc(graph->total_vertices * sizeof(int));
			int local_count = 0;

			// Each thread takes chunks from the frontier
			#pragma omp for schedule(dynamic) 
			for(int i = 0; i < current_frontier_size; i++) {
				int u = current_frontier[i];

				// Check the neigbors of each vertex in that chunk
				for (int j = graph->row_ptr[u]; j < graph->row_ptr[u + 1]; j++) {
					int v = graph->col_index[j];

					busy_work();

					if(v == target) {
						printf("Found!\n");
						#pragma omp atomic write
						found = 1;
						break;
					}
	
					if (!visited[v]) {
						// Prevent duplicates
						#pragma omp critical 
						{
							if (!visited[v]) {
								visited[v] = 1;
								local[local_count++] = v;
								nodes_checked++;
							}
						}
					}
				}
			}

			// Merge each thread’s local list
			#pragma omp critical
	        {
	            for(int i = 0; i < local_count; i++) {
					next_frontier[next_size + i] = local[i];
				}
	            next_size += local_count;
	        }

			free(local);
			
		}

		if(found)
			break;

			        
	    // Move to next level
	    memcpy(current_frontier, next_frontier, next_size * sizeof(int));
		
	    current_frontier_size = next_size;
		next_size = 0;
	    
		
	}
	

	if(!found)
		printf("Not Found!\n");
	
	printf("Nodes Checked: %d\n\n", nodes_checked);
	
	free(visited);
	free_graph(graph);
	free(current_frontier);
	free(next_frontier);
	
	return 0;
}




























