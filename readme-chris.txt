- To run it, just type "make run". This will auto recompile if changes were made.

- If you want to change vertex and edge count, edit the makefile. the bottom line shows the run command in order:
./"object" "vertices" "edges" "seed" "directed flag"

	- Next, we need to store this graph in a flat, compressed sparse row format in binary so that we can get it big enough that
	parallelizing has a noticeable effect and storing it isnt unreasonable. I think we should make a few huge graphs then just store
	them somewhere for testing throughout the project.
