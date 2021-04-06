/**
 * @brief source file for GVGraph class implementation
 * @author Peter Sun
 * @date 4-5-2021
 * @version 1.0
 */
#include "GVGraph.h"

GVGraph::GVGraph(vector<Edge> edgeList)
{
    set<string> vertices;
    //construct vertices for the graph
    for (auto e : edgeList)
    {
        //vertices is a set so there will be no duplicates of the same element
        vertices.insert(e.first);
        vertices.insert(e.second);
    }
    this->g = Graph(vertices.size()); //create instance of graph with the size of the vertices

    // construct two maps that mutually reference the name of the vertices and index of the vertices
    set<string>::iterator it;
    int vIndex = 0; // vertex number
    for (it = vertices.begin(); it != vertices.end(); ++it, vIndex++)
    {
        string vDescription = *it;
        boost::put(vertex_name_t(), this->g, vIndex, vDescription); // set the property of a vertex
        this->indexes[vDescription] = boost::vertex(vIndex, this->g);     // build map[node_description, vertex]
        this->lookup[boost::vertex(vIndex, this->g)] = vDescription;      // build map[vertex, node_description]
    }
    // add all the edges to the graph
    for (int i = 0; i < edgeList.size(); ++i)
    {
        add_edge(this->indexes[edgeList[i].first], this->indexes[edgeList[i].second], this->g);
    }
}

Graph GVGraph::computeDAG()
{

    struct Visitor : public boost::default_bfs_visitor {
        void discover_vertex(Vertex v, const Graph &) const {
            std::cout << "Current: " << v << std::endl;
        }
    };
    /*
Vertex start = 0;
        Visitor visitor;

         boost::breadth_first_search(g, start, boost::visitor(visitor));

        
        
        
         return g;
         */
    return NULL;
}