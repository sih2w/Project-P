#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

typedef struct {
    int From;
    int* To;
} Node;

typedef struct {
    int VerticeCount;
    int EdgeCount;
    int Seed;
    int ThreadCount;
    int Directed;
} Params;

typedef struct {
    int Seed;
    int Directed;
    Node* Tree;
    int EdgeCount;
    int StartVertexIndex;
    int EndVertexIndex;
} Worker;

int indexed(const int* connected_nodes, const int length, const int vertex_index) {
    for (int index = 0; index < length; index++) {
        if (connected_nodes[index] == vertex_index) return 1;
    }
    return 0;
}

int connect(Node* tree, Node* node) {

}

void* create_subtree(void* arg) {
    Worker* worker = (Worker*) arg;

    // Initialize each node in the subtree.
    for (int index = worker->StartVertexIndex; index < worker->EndVertexIndex; index++) {
        Node* node = (Node*) malloc(sizeof(Node));
        node->From = worker->StartVertexIndex;
        worker->Tree[index] = *node;
    }

    int edges_remaining = worker->EdgeCount;
    const int length = worker->EndVertexIndex - worker->StartVertexIndex;
    int* connected_nodes = (int*) malloc(sizeof(int) * length);

    // Randomly connect nodes together.
    while (edges_remaining > 0) {
        for (int index = worker->StartVertexIndex; index < worker->EndVertexIndex; index++) {
            int to_node_index = connect(worker->Tree, &worker->Tree[index]);

            edges_remaining--;
        }
    }

    // Connect nodes that were never connected.
    for (int index = worker->StartVertexIndex; index < worker->EndVertexIndex; index++) {
        if (!indexed(connected_nodes, length, index)) {

        }
    }

    free(connected_nodes);
    pthread_exit(NULL);

    return NULL;
}

Node* create_tree(const Params params) {
    Node* tree = (Node*) malloc(sizeof(Node) * params.VerticeCount);
    const int vertice_base = params.VerticeCount / params.ThreadCount;
    const int vertice_remainder = params.VerticeCount % params.ThreadCount;
    const int edge_base = params.EdgeCount / params.ThreadCount;
    const int edge_remainder = params.EdgeCount % params.ThreadCount;

    Worker** workers = malloc(sizeof(Worker*) * params.ThreadCount);
    pthread_t* threads = malloc(sizeof(pthread_t) * params.ThreadCount);

    for (int index = 0, vertex_index = 0; index < params.ThreadCount; index++) {
        const int vertices_for_worker = vertice_base + (index < vertice_remainder ? 1 : 0);
        const int edges_for_worker = edge_base + (index < edge_remainder ? 1 : 0);
        const int start_vertex_index = vertex_index;
        const int end_vertex_index = vertex_index + vertices_for_worker - 1;

        vertex_index = end_vertex_index + 1;

        Worker* worker = (Worker*) malloc(sizeof(Worker));
        worker->Tree = (Node*) malloc(sizeof(Node) * vertices_for_worker);
        worker->Seed = params.Seed;
        worker->Directed = params.Directed;
        worker->StartVertexIndex = start_vertex_index;
        worker->EndVertexIndex = end_vertex_index;
        worker->EdgeCount = edges_for_worker;

        workers[index] = worker;
        threads[index] = pthread_create(&threads[index], NULL, create_subtree, workers[index]);
    }

    for (int index = 0; index < params.ThreadCount; index++) {
        pthread_join(threads[index], NULL);
    }

    free(threads);
    free(workers);

    return tree;
}

int main(const int argc, char **argv) {
    Params params;
    params.ThreadCount = 1;
    params.Seed = 0;
    params.VerticeCount = 1000;
    params.EdgeCount = 10000;
    params.Directed = 0;

    return 0;
}