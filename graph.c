// gen_er.c — ER generator streaming "u v". Uniform endpoints; skips self-loops.
// Usage: ./gen_er N M [seed=1] [undirected=1]
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// Random number generator using xor shift. Called when deciding where to attach new vertices.
uint64_t xorshift64(uint64_t *state) {
    uint64_t x = *state;

    x = x ^ (x << 13);
    x = x ^ (x >> 7);
    x = x ^ (x << 17);

    *state = x;
    return x;
}

// Main
int main(int argc, char **argv) {
	// Check for correct number of arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s N M [undirected=1] [seed=1]\n", argv[0]);
        return 1;
    }

    // Store number of vertices and edges as 64bit integer. strtoull Converts a character string, to an unsigned long long value. I'm not totally sure what the arguments mean.
	// Vertices - N
    uint64_t N = strtoull(argv[1], NULL, 10);
	// Edges - M
    uint64_t M = strtoull(argv[2], NULL, 10);

	// Seed for RNG (default = 1)
    uint64_t seed = 1ULL;
	
	// Optional seed flag
    if (argc > 3) {
        seed = strtoull(argv[3], NULL, 10);
    }

    // Undirected graph flag (default = 1)
    int undirected = 1;
    if (argc > 4) {
        undirected = atoi(argv[4]);
    }

	// Force connectivity with a random spanning tree. We want connectivity so the bfs is forced to look
	// through every node in one parse and can't skip around to disconnected sections. This will make it work harder.
	// For every vertex, it will guarentee a connect to some previous vertex. If we specify that we want more
	// vertices than edges will allow, it will prioritize connecting the vertices and basically ignore the edge count.
	// This should be fine since we just want something to test the bfs on.
	uint64_t used = 0;
	for (uint64_t i = 1; i < N; i = i + 1) {
	    uint64_t u = i;
	    uint64_t v = xorshift64(&seed) % i;   // connect this node to some previous node
	
	    // Print the edge u -> v
	    printf("%llu %llu\n", (unsigned long long)u, (unsigned long long)v);
	
	    // If the graph is undirected, also print v -> u
	    if (undirected != 0) {
	        printf("%llu %llu\n", (unsigned long long)v, (unsigned long long)u);
	    }
	
	    // Increment the count of unique edges we have added
	    used = used + 1;
	}
	
	// Add the remaining random edges so that the total unique edges ≈ M. This will only happen if we have more edges
	// than vertices, and isnt all that important, just covers bases.
	uint64_t remaining;
	if (M > used) {
	    remaining = M - used;
		printf("Theres still edges left\n");
	} else {
	    remaining = 0;
	}

	// Create new vertices to accomodate the edges
	for (uint64_t i = 0; i < remaining; i = i + 1) {
	    uint64_t u = xorshift64(&seed) % N;
	    uint64_t v = xorshift64(&seed) % N;
	
	    // avoid self-loops
	    if (u == v) {
	        // instead of decrementing i (--i), just skip this iteration by reducing i
	        // so that we still produce the intended number of edges
	        if (i > 0) {
	            i = i - 1;
	        }
	        continue;
	    }
	
	    // print the edge u -> v
	    printf("%llu %llu\n", (unsigned long long)u, (unsigned long long)v);
	
	    // if the graph is undirected, also print v -> u
	    if (undirected != 0) {
	        printf("%llu %llu\n", (unsigned long long)v, (unsigned long long)u);
	    }
	}

	// This just prints the graph to the terminal where each pair represents an edge. You can draw it out and you'll see
	// that it makes a connected graph we can search on. You can edit the number of vertices and edges using command line arguments
	// manually or in the makefile.

	// Next, we need to store this graph in a flat, compressed sparse row format in binary so that we can get it big enough that
	// parallelizing has a noticeable effect and storing it isnt unreasonable. I think we should make a few huge graphs then just store
	// them somewhere for testing throughout the project.


    return 0;
}

