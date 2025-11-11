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
- MPI / OpenMP / Pthreads
- Linux/Unix environment recommended

## Parameters
- To change graph size/parameters, update ./graph in 'run' section in makefile. First command line argument is vertexes, second is edges, third is seed.
- To change target search value, edit the top of 'search_bfs.c'.

## How to Build
```bash
# To compile and run graph builder (serial):
make graph

# To compile bfs files:
make

# To run serial and parallel search on the graph:
make run
