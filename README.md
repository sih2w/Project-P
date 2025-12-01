# Parallel Processing Project

This repository contains the course project for **CSCI 6330 – Parallel Processing** with **Dr. Khem Poudel**.

## Course Information
- **Course:** CSCI 6330 – Parallel Processing  
- **Instructor:** Dr. Khem Poudel  
- **Semester:** Fall 2025 

## Project Description
This project explores topics in **parallel algorithms** and **parallel computing models**, with a focus on applying theoretical concepts to practical implementations in C.  

## Requirements
- C compiler (e.g., `gcc`)  
- OpenMP
- Lua/Roblox IDE
- Linux/Unix environment recommended

## Parameters
- To change graph size/parameters:
    - Edit './graph' line in 'run' section in makefile. First argument is vertexes, second is edges, third is seed.
- To change number of threads in parallel search:
    - Edit the top of parallel_bfs.c
      
- **GRAPH MUST BE BUILT BEFORE SEARCHING**
    - Instructions below  

## How to Build
```bash
# To compile and run graph builder (serial):
make graph

# To compile bfs files:
make

# To run serial search:
make runs

# To run parallel search:
make runp

# To run serial and parallel search on the graph:
make run

# To run a series of parallel searches:
make runpt
