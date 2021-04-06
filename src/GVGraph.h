/**
 * @brief source file for GVGraph class declaration
 * @author Peter Sun
 * @date 4-5-2021
 * @version 1.0
 */
#include <boost/algorithm/string.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>

using namespace std;
using namespace boost;

//define a custom type of Edge for pair<string,string>
typedef std::pair<std::string, std::string> Edge;
//define a custom type of VertexProperty
typedef property<vertex_name_t, std::string> VertexProperty;
//define a custom type of Graph for undirected adjacency_list
typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty> Graph;
//using Vertex for vertex_descriptor which is a number
using Vertex = Graph::vertex_descriptor;
/**
 * @brief The GVGraph class implements Girvan-Newman algorithm using Boost Graphic Library
 */
class GVGraph
{
private:
    Graph g; //Represents the graph of the input connection data
    std::map<std::string, Vertex> indexes; //key: text name of the vertex, value: number index of the vertex
    std::map<Vertex, std::string> lookup; //key: number index of the vertex, value: text name of the vertex

    Graph computeDAG();

public:
    GVGraph(vector<Edge> edgeList); //construct a graph with a list of social network connection edges
    ~GVGraph() {}
    Graph getGraph() { return g; } //return the original graph
};


