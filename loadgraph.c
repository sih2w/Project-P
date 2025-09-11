#include <stdio.h>
#include <stdlib.h>

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
    g->row_ptr   = malloc((g->edge_count) * sizeof(int));
    g->col_index = malloc(g->total_vertices * sizeof(int));
    if (!g->row_ptr || !g->col_index) {
        fclose(f);
        free(g->row_ptr); free(g->col_index); free(g);
        return NULL;
    }

    // read arrays
    fread(g->row_ptr,   sizeof(int), g->edge_count, f);
    fread(g->col_index, sizeof(int), g->total_vertices, f);

    fclose(f);
    return g;
}

void free_graph(CSR *g) {
    if (!g) return;
    free(g->row_ptr);
    free(g->col_index);
    free(g);
}

int main() {

	CSR *graph = load_graph("savefile");

	////////////////// Testing output ///////////////////////
	printf("\nrow_ptr:\n");
	// Print for testing
	for(int i = 0; i < graph->edge_count; i++) {
		printf("%d ", graph->row_ptr[i]);
	}
	printf("\ncol_index:\n");

	// Print for testing
	for(int i = 0; i < graph->total_vertices; i++) {
		printf("%d ", graph->col_index[i]);
	}
	printf("\n\n");

	free_graph(graph);
	
	
	return 0;
}
