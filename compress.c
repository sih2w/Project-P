#include <stdio.h>
#include <stdlib.h>

int main() {
	// Open File
	FILE *file = fopen("edgelist.txt", "r");
    if (!file) {
		perror("fopen"); return 1;
	}

	int total_vertices = 0;
	int u, v = -1;
	int edge_count = 0;

	// Initial pass through file
	// Count number of edges and highest vertex (# of vertices)
	while(fscanf(file, "%d %d", &u, &v) == 2) {
		if(u > total_vertices) {
			total_vertices = u;
		}
		if (v > total_vertices) {
			total_vertices = v;
		}
		edge_count++;
	}

	total_vertices++;

	////////////////// Testing output ///////////////////////
	printf("Max: %d\nEdges: %d\n", total_vertices - 1, edge_count);

	// Move scanf back to start
	fseek(file, 0, SEEK_SET);

	// We need to store how many neighbors each vertex has before we can make col_index[].
	// The number of neighbors is called a degree and we parse again to find it.
	int *deg = (int *)malloc(total_vertices * sizeof(int));
	while(fscanf(file, "%d %d", &u, &v) == 2) {
		deg[u]++;
	}

	////////////////// Testing output ///////////////////////
	for(int i = 0; i < total_vertices; i++) {
		printf("Vertex: %d - %d\n", i, deg[i]);
	}

	// row_ptr stores the neighbors of each vertex. Each vertex may have mulitple  neighbors so 
	// each one will take up different amounts of array space. Becasue of this we have to keep track of which
	// vertex has these neighbors using a second array, col_index.
	// int *row_ptr = calloc((2 * edge_count) * sizeof(int));
	int *row_ptr = calloc(edge_count, sizeof(int));

	// Init (remove)
	for(int i = 0; i < edge_count; i++) {
		row_ptr[i] = -1;
	}
	
	// col_index keeps track of the indexes that represent each vertex that has those neighbors in row_ptr.
	// Vertex 3 may have 2 neighbors *6 and *8. In row_ptr, we just see [...6, 8, ...] somewhere in the array but we don't
	// know whose neighbors they are. col_index will tell us this by keeping the index where the list of neighbors starts in
	// row_ptr at the corrosponding index of col_index. so col_index[3] (vertex 3) has value x, where x is the 
	// starting index of vertex 3's neighbors in row_ptr.
	// int *col_index = malloc(total_vertices * sizeof(int));
	int *col_index = calloc(total_vertices, sizeof(int));
	int pointer = 0;
	for(int i = 0; i < total_vertices; i++) {
		col_index[i] = pointer;
		pointer += deg[i];
	}

	// This keeps track of the next open neighbor space in row_ptr[]
	int *next = malloc(total_vertices * sizeof(int));




	// Move scanf back to start
	fseek(file, 0, SEEK_SET);

	// Now we need to make our final parse and put the neighbors of each vertex in its place within row_ptr.
	while(fscanf(file, "%d %d", &u, &v) == 2) {
		row_ptr[col_index[u] + next[u]] = v;
		next[u]++;
	}













	////////////////// Testing output ///////////////////////
	printf("\nrow_ptr:\n");
	// Print for testing
	for(int i = 0; i < edge_count; i++) {
		printf("%d ", row_ptr[i]);
	}
	printf("\ncol_index:\n");

	// Print for testing
	for(int i = 0; i < total_vertices; i++) {
		printf("%d ", col_index[i]);
	}
	printf("\n");
	printf("\n");


	////////////////// Testing output ///////////////////////
	for(int i = 0; i < total_vertices; i++) {
		next[i] = 0;
	}

	for(int i = 0; i < total_vertices - 1; i++) {
		printf("Vertex %2d - {", i);
		for(int j = 0; j < deg[i]; j++) {
			printf("%d ", row_ptr[col_index[i] + j]);
		}
		printf("}\n");
	}
	printf("\n");
	






	
	
	// Free dynamic memory
	free(deg);
	free(row_ptr);
	free(col_index);
	free(next);
	
	return 0;
}
