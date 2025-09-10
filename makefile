graph: graph.c
	gcc graph.c -o graph

compress: compress.c
	gcc compress.c -o compress

clean:
	rm -f graph rm -f *.o

rung: graph
	./graph 20 18 2348952347

runc: compress
	./compress
