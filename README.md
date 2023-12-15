# adjoint_line_graphs

#### Input: adjacency list of a directed graph(s)
Like so: 

![image](https://github.com/annnkle/adjoint_line_graphs/assets/95099151/bed4a07a-6a15-4e32-9d21-4026e856290e)

#### Algorithm:
1. Check whether a graph is a multigraph (e.g. has multiplies of edges) - graph cannot be adjoint then and algorithm ends
2. If not a multigraph, then check whether a graph is adjonint - if not, algorithm ends
3. If graph is ajdoint, check whether it's line.
4. Create original graph based on adjoint and save its adjacency list as output file.

#### Goal
Graphs of those qualities can be used in solving the assembly of nucleotide sequencies.
