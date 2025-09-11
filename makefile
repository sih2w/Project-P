graph: graph.c
	gcc graph.c -o graph

compress: compress.c
	gcc compress.c -o compress

load: loadgraph.c
	gcc loadgraph.c -o load

clean:
	rm -f graph rm -f *.o

rung: graph
	./graph 20 18 2348952347

runc: compress
	./compress

runl: load
	./load
