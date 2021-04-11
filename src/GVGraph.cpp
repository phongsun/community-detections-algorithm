/**
 * @brief source file for GVGraph class implementation
 * @author Peter Sun
 * @date 4-5-2021
 * @version 1.0
 */
#include "GVGraph.h"

GVGraph::GVGraph(vector<Edge> edgeList) {
    set<string> vertices;
    for (auto e : edgeList) {
        vertices.insert(e.first);
        vertices.insert(e.second);
    }
    this->g = Graph(vertices.size());

    // fills the property 'vertex_name_t' of the vertices
    set<string>::iterator it;
    int vIndex = 0; // vertex number
    for (it = vertices.begin(); it != vertices.end(); ++it, vIndex++) {
        string vDescription = *it;
        boost::put(vertex_name_t(), this->g, vIndex, vDescription);   // set the property of a vertex
        this->indexes[vDescription] = boost::vertex(vIndex, this->g); // build map[node_description, vertex]
        this->lookup[boost::vertex(vIndex, this->g)] = vDescription;  // build map[vertex, node_description]
    }

    for (int i = 0; i < edgeList.size(); ++i) {
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
        for (boost::tie(ei, ei_end) = edges(dag); ei != ei_end; ++ei) {
            // find the edge in g that is the same edge as the current dag edge
            int s = source(*ei, dag);
            int t = target(*ei, dag);
            auto edgeData = boost::edge(s, t, g);
            float dagBtw = get(edge_weight_t(), dag, *ei);
            float gBtw = get(edge_weight_t(), g, edgeData.first);
            put(edge_weight_t(), g, edgeData.first, dagBtw + gBtw);
        }
    }

    // divide total betweenesses of each edge by 2
    graph_traits<Graph>::edge_iterator gei, gei_end;
    for (boost::tie(gei, gei_end) = edges(this->g); gei != gei_end; ++gei) {
        // find weight of current edge and divide it by 2
        float gBtw = get(edge_weight_t(), g, *gei);
        gBtw = gBtw / 2.0;
        put(edge_weight_t(), g, *gei, gBtw);
        // put weight and the corresponding edge into a map for later edge removal
        if (this->btw_map.find(gBtw) == this->btw_map.end()) {
            this->btw_map[gBtw] = set<pair<int, int>>();
        }
        this->btw_map[gBtw].insert(pair<int, int>(source(*gei, g), target(*gei, g)));
    }

    return g;
}

DAG GVGraph::computeDAG(Vertex start) {
    typedef graph_traits<DAG>::out_edge_iterator out_edge_it;
    typedef graph_traits<DAG>::in_edge_iterator in_edge_it;

    // BFS DAG
    DAG dag = DAG(num_vertices(g));
    // map is used to track vertices at each level
    map<int, set<Vertex>> levelMap = map<int, set<Vertex>>();
    levelMap[0] = set<Vertex>({start});
    Visitor visitor(dag, start, levelMap);

    // 1. BFS from the starting vertex and have the visitor to construct a DAG
    boost::breadth_first_search(g, start, boost::visitor(visitor));

    // 2. top-down: calculate a shortest path count for each vertex
    for (int i = 1; i < levelMap.size(); i++) {
        for (auto node : levelMap[i]) {
            out_edge_it ei, ei_end;
            for (boost::tie(ei, ei_end) = out_edges(node, dag); ei != ei_end; ++ei) {
                Vertex t = target(*ei, dag);
                // skip edges in the DAG if they are on the same level
                if (!levelMap[i].count(t)) {
                    // a node adds its own shortest path count to its child's existing shortest path count
                    int shortestPathCount =
                            get(boost::vertex_rank_t(), dag, t) + get(boost::vertex_rank_t(), dag, node);
                    boost::put(vertex_rank_t(), dag, t, shortestPathCount);
                }
            }
        }
    }

    // 3. bottom-up: calculate betweeness for each edge
    for (int i = levelMap.size() - 1; i > 0; i--) {
        for (auto node : levelMap[i]) {
            // each node get a credit of 1 + sum of out edges weight
            float nodeCredit = 1;
            out_edge_it ei, ei_end;
            for (boost::tie(ei, ei_end) = out_edges(node, dag); ei != ei_end; ++ei) {
                float wt = get(boost::edge_weight_t(), dag, *ei);
                nodeCredit += wt;
            }

            // sum shortest_path_count of all in_edge nodes (sum_s_p_c),
            in_edge_it in_ei, in_ei_end;
            float totalSPC = 0;
            for (boost::tie(in_ei, in_ei_end) = in_edges(node, dag); in_ei != in_ei_end; ++in_ei) {
                Vertex parent = source(*in_ei, dag);
                if (!levelMap[i].count(parent)) { // skip if parent is a the same level
                    int spc = get(vertex_rank_t(), dag, source(*in_ei, dag));
                    totalSPC += spc;
                }
            }

            // in_edge weight = s_p_c/sum_s_p_c + credit
            for (boost::tie(in_ei, in_ei_end) = in_edges(node, dag); in_ei != in_ei_end; ++in_ei) {
                Vertex parent = source(*in_ei, dag);
                if (!levelMap[i].count(parent)) { // skip if parent is a the same level
                    int spc = get(vertex_rank_t(), dag, source(*in_ei, dag));
                    float wt = spc / totalSPC * nodeCredit;
                    put(edge_weight_t(), dag, *in_ei, wt);
                }
            }
        }
    }

    return dag;
}

float GVGraph::computeModularity(map<int, set<int>> clusters) {
    float B = 0;
    // _m = num_edges(g)
    // for each cluster in the clusters
    set<int>::iterator it1, it2;
    for (auto const &[clusterId, vertices] : clusters) {
        for (it1 = vertices.begin(); it1 != std::prev(vertices.end()); ++it1) {
            //   for each vertex u, get degree (ud)
            float u_d = in_degree(*it1, g);
            for (it2 = std::next(it1, 1); it2 != vertices.end(); it2++) {
                // for rest of vertex v, in the component, get degree (vd)
                float v_d = in_degree(*it2, g);
                // probability p_vu = (vd*ud)/(2*m)
                float p_uv = (v_d * u_d) / (2 * _m);
                // if (v, u) is an edge of g, b_v = 1 - p_vu, else b_v = -p_vu
                float b_uv = boost::edge(*it1, *it2, g).second ? 1.0 - p_uv : -1.0 * p_uv;
                B = B + b_uv;
            }
        }
    }
    float Q = B / (2 * _m);
    return Q;
}

pair<Graph, float> GVGraph::detectCommunities() {
    Graph communities;
    typedef graph_traits<Graph>::edge_iterator edge_it_g;
    int last_num_components = 1;
    float modulairty = -1;
    float Q = -1;

    // 1. compute betweenness for the graph
    this->g = computeBetweeness();

    // 2. remove highest btw edges till the graph split
    for (auto const &pair : this->btw_map) {
        auto btw = pair.first;
        // remove edges with btw
        for (auto const &e_btw : pair.second) {
            // if edge exist
            if (edge(e_btw.first, e_btw.second, g).second) {
                // remove the edge
                remove_edge(edge(e_btw.first, e_btw.second, g).first, g);
            }
        }
        // reset the _m after the edge removal
        _m = num_edges(g);
        if (_m == 0) {
            break;
        }
        // find disconnected clusters in the graph
        map<int, int> subClusters;
        int nclusters = connected_components(this->g, make_assoc_property_map(subClusters));

        // if number of sub cluster is more than last round, compute Q
        if (nclusters > last_num_components) {
            // convert subClusters to a easier data structure
            map<int, set<int>> clusterMap = convertMap(subClusters);

            // 3. calculate modularity of the components in the graph
            Q = this->computeModularity(clusterMap);
        }
        // take the graph of clusters with the best modularity
        if (Q > modulairty) {
            communities = Graph();
            modulairty = Q;
            boost::copy_graph(g, communities);
        }
        last_num_components = nclusters;
    }

    return pair < Graph, float > (communities, modulairty);
}