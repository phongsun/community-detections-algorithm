# Project 03 - Community Detection

**Due: Monday April 12, 2021 @ 6am**

## Overview

Community detection in networks is a popular area of graph theory.  The Girvan-Newman algorithm is a relatively new option for performing community detection in networks/graphs.  

For this project, you'll implement the Girvan-Newman algorithm using the Boost Graph Library.  You can find more about Girvan-Newman in the original paper and also some other resources:
* Girvan M. and Newman M. E. J., [Community structure in social and biological networks](https://www.pnas.org/content/99/12/7821), Proc. Natl. Acad. Sci. USA 99, 7821–7826 (2002)
* Chapter 10 Section 2 of [Mining Massive Data Sets](http://infolab.stanford.edu/~ullman/mmds/book0n.pdf)
* [Social and Information Network Analysis Course Slide Deck 14](http://snap.stanford.edu/class/cs224w-2010/slides/14-communities_annot.pdf)


What you need to do:

  1. read about Girvan Newman and figure out how the algorithm works. 
  2. create a new project and explore the Boost Graph Library
  3. implement
    1. reading in the network/graph file based on the format below
    2. detect communities
    3. output the communities detected into a separate file.  Each community should have a heading `Community X` followed by a list of nodes that are part of that community each on a separate line. 

You can use the [NetworkX](https://networkx.org/), a Python network analysis library to generate graphs to use for testing.  Take a look at the Graph Generator functionality here The library also has 4 social network graphs already included.  You'll just need to write them out to a file in the format below so your program can read them. 

Example network input:

```text
9
A - B
A - C
B - C
B - D
D - G
D - F
D - E
G - F
E - F
```

Important:  You'll need to include a Github actions workflow file so that your the project will build under Actions.

# What is this?
This program implements Girvan-Newman algorithm in C++ using the Boost Graphic Library in the following steps:
1. Create DAGs using BFS for each vertex in the original graph
2. Calculate shortest paths from the root through each vertex. The shortest path is stored in the vertex_rank property
3. Calculate betweeness of each edge by walking up the level of the DAG. Betweenness is stored in the edge_weight property
4. Aggregate betweeness of corresponding edges of each DAG and assign them back to the original graph
5. Remove edges by betweenness in decending order
  4.1. if a removal creates a new cluster in the graph, calculate modularity of the group of clusters in the graph
  4.2. record the highest modularity so far
  4.3. stop when all edges are removed
6. The resulting communities is the group of clusters with the highest modularity

The program has a class called GVGraph which implements the above steps. The GVGraph class also has an inner struct called Visitor which implements BGL's default_bfs_vistor and creates a DAG from a vertex of the original graph. The Visitor struct also creates a map that tracks levels of the DAG and vertices at each level.

The calculations are tested against examples walked through in class and lecture notes from other universities. 
1. Calcultion of the count of shortest paths is tested against slide 36 in http://www.cs.uoi.gr/~tsap/teaching/2016-cse012/slides/datamining-lect12.pdf
2. Calculation of betweenesses of DAG is tested against slide 37 in http://www.cs.uoi.gr/~tsap/teaching/2016-cse012/slides/datamining-lect12.pdf
3. Calculation of aggregated betweeness is tested against slide 41 in http://www.cs.uoi.gr/~tsap/teaching/2016-cse012/slides/datamining-lect12.pdf
4. Calculation of modularity is tested against slides in https://nms.kcl.ac.uk/colin.cooper/teachingmaterial/CSMWAL/CSMWAL/Lectures/CommunitySlides.pdf

# How do I install it?
0. make sure you have g++17 installed and cd $install_dir
1. clone https://github.com/smu-cs-3353/21s-pa03-peter-sun.git
2. cd $install_dir/21s-pa02-phongsun
3. mkdr build
4. cd build
5. cmake .. --config Release
6. make
7. cd ..
8. ls social-graph 
- You should find social-graph in the directory.

# How do I try it out?
0. cd $install_dir/21s-pa02-phongsun/data
1. edit generate_input_datafile.py to select your favorite social network sample data (default is les_miserables_graph)
3. python generate_input_datafile.py
4. check data in put_data_here.txt
5. cd ..
6. ./social-graph put_data_here.txt
7. in terminal, you should see *** x communities have been detected for y vertices *** 
8. cat output.txt
- You should find detected communities in the file


# Which versions of C++ does it work with?
C++ 17

# What is the license?
GPL
