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
    for (auto e : edgeList)
    {
        vertices.insert(e.first);
        vertices.insert(e.second);
    }
    this->g = Graph(vertices.size());

    // fills the property 'vertex_name_t' of the vertices
    set<string>::iterator it;
    int vIndex = 0; // vertex number
    for (it = vertices.begin(); it != vertices.end(); ++it, vIndex++)
    {
        string vDescription = *it;
        boost::put(vertex_name_t(), this->g, vIndex, vDescription);   // set the property of a vertex
        this->indexes[vDescription] = boost::vertex(vIndex, this->g); // build map[node_description, vertex]
        this->lookup[boost::vertex(vIndex, this->g)] = vDescription;  // build map[vertex, node_description]
    }

    for (int i = 0; i < edgeList.size(); ++i)
    {
        add_edge(this->indexes[edgeList[i].first], this->indexes[edgeList[i].second], this->g);
    }
}
Graph GVGraph::computeBetweeness() {
    typedef graph_traits<DAG>::edge_iterator edge_it;
    // iterate through vertices
    for (auto vd : boost::make_iterator_range(boost::vertices(this->g))) {
        // compute individual DAG from each vertex with betweenesses
        DAG dag = computeDAG(vd);
        // aggregate individual betweenesses of each DAG to g
        edge_it ei, ei_end;
        for (boost::tie(ei, ei_end) = edges(dag); ei != ei_end; ++ei)
        {
            //float btw = boost::get(vertex_distance_t(), dag, *ei);
        }
    }


    // divide total betweenesses of each edge by 2
    return g;
}

DAG GVGraph::computeDAG(Vertex start)
{
    typedef graph_traits<DAG>::out_edge_iterator out_edge_it;
    typedef graph_traits<DAG>::in_edge_iterator in_edge_it;

    // BFS DAG
    DAG dag = DAG(num_vertices(g));
    // map is used to track vertices at each level
    map<int, set<Vertex>> levelMap = map<int, set<Vertex>>();
    levelMap[0] = set<Vertex>({start});
    Visitor visitor(dag, start, levelMap);

    // 1. BFS from A and have the visitor to construct a DAG
    boost::breadth_first_search(g, start, boost::visitor(visitor));

    // 2. top-down: calculate a shortest path count for each vertex
    for (int i = 1; i < levelMap.size(); i++)
    {
        for (auto node : levelMap[i])
        {
            out_edge_it ei, ei_end;
            for (boost::tie(ei, ei_end) = out_edges(node, dag); ei != ei_end; ++ei)
            {
                Vertex t = target(*ei, dag);
                // skip edges in the DAG if they are on the same level
                if (!levelMap[i].count(t))
                {
                    int shortestPathCount = get(boost::vertex_rank_t(), dag, t) + get(boost::vertex_rank_t(), dag, node);
                    boost::put(vertex_rank_t(), dag, t, shortestPathCount);
                }
            }
        }
    }

    // 3. bottom-up: calculate betweeness for each edge
    for (int i = levelMap.size() - 1; i > 0; i--)
    {
        for (auto node : levelMap[i])
        {
            // each node get a credit of 1 + sum of out edges weight
            float nodeCredit = 1;
            out_edge_it ei, ei_end;
            for (boost::tie(ei, ei_end) = out_edges(node, dag); ei != ei_end; ++ei)
            {
                float wt = get(boost::edge_weight_t(), dag, *ei);
                nodeCredit += wt;
            }

            // sum shortest_path_count of all in_edge nodes (sum_s_p_c),
            in_edge_it in_ei, in_ei_end;
            float totalSPC = 0;
            for (boost::tie(in_ei, in_ei_end) = in_edges(node, dag); in_ei != in_ei_end; ++in_ei)
            {
                Vertex parent = source(*in_ei, dag);
                if (!levelMap[i].count(parent))
                { // skip if parent is a the same level
                    int spc = get(vertex_rank_t(), dag, source(*in_ei, dag));
                    totalSPC += spc;
                }
            }

            // in_edge weight = s_p_c/sum_s_p_c + credit
            for (boost::tie(in_ei, in_ei_end) = in_edges(node, dag); in_ei != in_ei_end; ++in_ei)
            {
                Vertex parent = source(*in_ei, dag);
                if (!levelMap[i].count(parent))
                { // skip if parent is a the same level
                    int spc = get(vertex_rank_t(), dag, source(*in_ei, dag));
                    float wt = spc / totalSPC * nodeCredit;
                    put(edge_weight_t(), dag, *in_ei, wt);
                }
            }
        }
    }

    std::cout << "edges(dag) = ";
    graph_traits<DAG>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(dag); ei != ei_end; ++ei)
    {
        float wt = get(boost::edge_weight_t(), dag, *ei);
        std::cout << "("
                  << lookup[source(*ei, dag)]
                  << "," << lookup[target(*ei, dag)]
                  << ", " << wt << ")\n";
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

    return dag;
}
