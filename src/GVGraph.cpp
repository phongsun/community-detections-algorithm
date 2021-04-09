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

Graph GVGraph::computeBetweeness()
{
    typedef graph_traits<DAG>::out_edge_iterator out_edge_it;
    typedef graph_traits<DAG>::in_edge_iterator in_edge_it;

    Vertex start = this->indexes["A"];
    // BFS DAG
    DAG dag = DAG(num_vertices(g));
    // map is used to track vertices at each level
    map<int, set<Vertex>> levelMap = map<int, set<Vertex>>();
    levelMap[0] = set<Vertex>({start});
    Visitor visitor(dag, start, levelMap);

    // 1. BFS from A and have the vistor to construct a DAG
    boost::breadth_first_search(g, start, boost::visitor(visitor));

    // 2. top-down: calculate a shortest path count for each vertex
    for (int i = 1; i < levelMap.size(); i++) {
        for(auto node: levelMap[i]) {
            out_edge_it ei, ei_end;
            for (boost::tie(ei, ei_end) = out_edges(node, dag); ei != ei_end; ++ei){
                Vertex t = target(*ei, dag);
                // skip edges in the DAG if they are on the same level
                if (!levelMap[i].count(t)) {
                    int shortestPathCount = get(boost::vertex_rank_t(), dag, t) + get(boost::vertex_rank_t(), dag, node);
                    boost::put(vertex_rank_t(), dag, t, shortestPathCount);
                }
            }
        }
    }

    // 3. bottom-up: calculate betweeness for each edge
    for (int i = levelMap.size() - 1; i > 0; i--) {
        for(auto node: levelMap[i]) {
            // each node get a credit of 1 + sum of out edges weight
            float nodeCredit = 1;
            out_edge_it ei, ei_end;
            for (boost::tie(ei, ei_end) = out_edges(node, dag); ei != ei_end; ++ei){
                float wt = get(boost::edge_weight_t(), dag, *ei);
                nodeCredit += wt;
            }

            /*in_edge_it in_ei, in_ei_end;
            for (boost::tie(in_ei, in_ei_end = in_edges(node, dag); in_ei != in_ei_end; ++in_ei){
                float wt = get(boost::edge_weight_t(), dag, *ei);
                nodeCredit += wt;
            }*/

            // sum shortest_path_count of all in_edge nodes (sum_s_p_c),

            // in_edge weight = s_p_c/sum_s_p_c + credit
            /*graph_traits<DAG>::out_edge_iterator i_ei, i_ei_end;
            for (boost::tie(i_ei, i_ei_end) = out_edges(node, dag); i_ei != i_ei_end; ++i_ei){
                Vertex t = target(*i_ei, dag);
                // skip edges in the DAG if they are on the same level
                if (!levelMap[i].count(t)) {
                    int shortestPathCount = get(boost::vertex_rank_t(), dag, t) + get(boost::vertex_rank_t(), dag, node);
                    boost::put(vertex_rank_t(), dag, t, shortestPathCount);
                }
            }*/
        }
    }

    // calculate shortest path count for each node


    std::cout << "edges(dag) = ";
    graph_traits<DAG>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(dag); ei != ei_end; ++ei)
    {
        std::cout << "("
                  << lookup[source(*ei, dag)]
                  << "," << lookup[target(*ei, dag)]
                  << ") ";
    }
    std::cout << std::endl;
    for (auto const &pair : levelMap)
    {
        cout << "level - " << pair.first << endl;

        auto vList = pair.second;
        for (auto x : vList)
        {
            int p = boost::get(boost::vertex_rank_t(), dag, x);
            cout << lookup[x] << "(" << p << ") ";
        }
        std::cout << std::endl;
    }

    return g;
}