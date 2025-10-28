// gen_er.c — ER generator streaming "u v". Uniform endpoints; skips self-loops.
// Usage: ./gen_er N M [undirected=1] [seed=1]
// Build: gcc -O3 -march=native -pipe gen_er.c -o gen_er
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Random number generator using xor shift
uint64_t xorshift64(uint64_t *state) {
    uint64_t x = *state;
    x ^= (x << 13);
    x ^= (x >> 7);
    x ^= (x << 17);
    *state = x;
    return x;
}

// Main
int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s N M [undirected=1] [seed=1]\n", argv[0]);
        return 1;
    }

    uint64_t N = strtoull(argv[1], NULL, 10);
    uint64_t M = strtoull(argv[2], NULL, 10);

    uint64_t seed = 1ULL;
    if (argc > 3) {
        seed = strtoull(argv[3], NULL, 10);
    }

    int undirected = 1;
    if (argc > 4) {
        undirected = atoi(argv[4]);
    }

    // Open file for writing
    FILE *fp = fopen("edgelist.txt", "w");
    if (!fp) {
        perror("Error opening edgelist.txt");
        return 1;
    }

    uint64_t used = 0;
    for (uint64_t i = 1; i < N; i++) {
        uint64_t u = i;
        uint64_t v = xorshift64(&seed) % i;

        fprintf(fp, "%llu %llu\n", (unsigned long long)u, (unsigned long long)v);
        if (undirected != 0) {
            fprintf(fp, "%llu %llu\n", (unsigned long long)v, (unsigned long long)u);
        }
        used++;
    }

    uint64_t remaining = (M > used) ? (M - used) : 0;

    for (uint64_t i = 0; i < remaining; i++) {
        uint64_t u = xorshift64(&seed) % N;
        uint64_t v = xorshift64(&seed) % N;

        if (u == v) {
            if (i > 0) i--;
            continue;
        }

        fprintf(fp, "%llu %llu\n", (unsigned long long)u, (unsigned long long)v);
        if (undirected != 0) {
            fprintf(fp, "%llu %llu\n", (unsigned long long)v, (unsigned long long)u);
        }
    }

    fclose(fp);

    return 0;
}
