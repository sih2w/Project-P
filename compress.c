#include <stdio.h>
#include <stdlib.h>

void save_graph(const char* filename, int total_vertices, int edge_count, int* row_ptr, int* col_index) {
    FILE *f = fopen(filename, "wb");   // open binary file for writing
    if (!f) {
        perror("fopen");
        return;
    }

    // write vertex count
    fwrite(&total_vertices, sizeof(total_vertices), 1, f);
    // write edge count
    fwrite(&edge_count, sizeof(edge_count), 1, f);

    // write row_ptr array (V+1 elements)
    fwrite(row_ptr, sizeof(int), total_vertices + 1, f);

    // write col_index array (E elements)
    fwrite(col_index, sizeof(int), edge_count, f);

    fclose(f);
}

int main() {
    // Open File
    FILE *file = fopen("edgelist.txt", "r");
    if (!file) {
        perror("fopen"); return 1;
    }

    int total_vertices = 1;
    int u, v = -1;
    int edge_count = 0;

    // Initial pass through file
    while(fscanf(file, "%d %d", &u, &v) == 2) {
        if(u > total_vertices) total_vertices = u;
        if(v > total_vertices) total_vertices = v;
        edge_count++;
    }
    total_vertices++;

    fseek(file, 0, SEEK_SET);

    int *deg = (int *)calloc(total_vertices, sizeof(int));
    while(fscanf(file, "%d %d", &u, &v) == 2) {
        deg[u]++;
    }

    // Correct CSR sizes
    int *row_ptr = calloc(total_vertices + 1, sizeof(int));
    int *col_index = calloc(edge_count, sizeof(int));

    int pointer = 0;
    for(int i = 0; i < total_vertices; i++) {
        row_ptr[i] = pointer;
        pointer += deg[i];
    }
    row_ptr[total_vertices] = edge_count;

    int *next = calloc(total_vertices, sizeof(int));

    fseek(file, 0, SEEK_SET);

    while(fscanf(file, "%d %d", &u, &v) == 2) {
        col_index[row_ptr[u] + next[u]] = v;
        next[u]++;
    }

    save_graph("savefile", total_vertices, edge_count, row_ptr, col_index);

    free(deg);
    free(row_ptr);
    free(col_index);
    free(next);
    fclose(file);

    return 0;
}
