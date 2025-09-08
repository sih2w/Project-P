graph: graph.c
	gcc graph.c -o graph

clean:
	rm -f graph rm -f *.o

run: graph
	./graph 20 20 2348952347
