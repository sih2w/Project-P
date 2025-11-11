SHELL := /bin/bash

default:
	gcc serial_bfs.c -o serial
	gcc -fopenmp parallel_bfs.c -o parallel


clean:
	rm -f graph rm -f *.o
	rm -f loadgraph
	rm -f compress
	rm -f serial
	rm -f parallel

run:
	@start=$$(date +%s%N); \
	./serial 111111; \
	end=$$(date +%s%N); \
	elapsed=$$(echo "scale=3; ($$end - $$start)/1000000000" | bc); \
	echo "Serial Search Time: $$elapsed seconds"
	@start=$$(date +%s%N); \
	./parallel 111111; \
	end=$$(date +%s%N); \
	elapsed=$$(echo "scale=3; ($$end - $$start)/1000000000" | bc); \
	echo "Parallel Search Time: $$elapsed seconds"

runs:
	@start=$$(date +%s%N); \
	./serial 111111; \
	end=$$(date +%s%N); \
	elapsed=$$(echo "scale=3; ($$end - $$start)/1000000000" | bc); \
	echo "Serial Search Time: $$elapsed seconds"

runp: parallel
	@start=$$(date +%s%N); \
	./parallel 111111; \
	end=$$(date +%s%N); \
	elapsed=$$(echo "scale=3; ($$end - $$start)/1000000000" | bc); \
	echo "Parallel Search Time: $$elapsed seconds"

graph:
	@gcc graph.c -o graph
	@gcc compress.c -o compress
	@echo "Creating Graph..." 
	@start=$$(date +%s%N); \
	./graph 2000000 0 711232; \
	./compress; \
	end=$$(date +%s%N); \
	elapsed=$$(echo "scale=3; ($$end - $$start)/1000000000" | bc); \
	echo "Generation Time: $$elapsed seconds"
