SHELL := /bin/bash

default: graph.c compress.c loadgraph.c
	gcc graph.c -o graph
	gcc compress.c -o compress
	gcc loadgraph.c -o loadgraph

graph: graph.c
	gcc graph.c -o graph

compress: compress.c
	gcc compress.c -o compress

load: loadgraph.c
	gcc loadgraph.c -o loadgraph

search: serial_bfs.c
	gcc serial_bfs.c -o search

clean:
	rm -f graph rm -f *.o
	rm -f loadgraph
	rm -f compress

rung: graph
	./graph 100 0 2348952347

runc: compress
	./compress

runl: load
	./load

runs: search
	@start=$$(date +%s%N); \
	./search; \
	end=$$(date +%s%N); \
	elapsed=$$(echo "scale=3; ($$end - $$start)/1000000000" | bc); \
	echo "Search Time: $$elapsed seconds"

run:
	@gcc graph.c -o graph
	@gcc compress.c -o compress
	@echo "Creating Graph..." 
	@start=$$(date +%s%N); \
	./graph 2000000 0 711232; \
	./compress; \
	end=$$(date +%s%N); \
	elapsed=$$(echo "scale=3; ($$end - $$start)/1000000000" | bc); \
	echo "Generation Time: $$elapsed seconds"
