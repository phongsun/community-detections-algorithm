# This python program generates the input data file using networkx's graph generator:
# Use the following URL to select a sample data graph such as les_miserables_graph
# https://networkx.org/documentation/stable/reference/generators.html
import networkx as nx

G = nx.les_miserables_graph()
#G = nx.karate_club_graph()

input_file = open("../cmake-build-debug/data/put_data_here.txt", "w")
print(G.number_of_edges())
input_file.write(f"{G.number_of_edges()}\n")
for u,v,a in G.edges(data=True):
    input_file.write(f"{u} - {v}\n")
input_file.close()