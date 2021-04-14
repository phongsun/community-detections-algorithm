/**
 * @brief source file for GVGraph class declaration
 * @author Peter Sun
 * @date 4-5-2021
 * @version 1.0
 */
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/copy.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>

using namespace std;
using namespace boost;

typedef std::pair<std::string, std::string> Edge;
typedef property<vertex_name_t, std::string> VertexName;
typedef property<edge_weight_t, float> EdgeWeight;
typedef adjacency_list<vecS, vecS, undirectedS, VertexName, EdgeWeight> Graph;
typedef property<vertex_rank_t, int> VertexShortestPathCount;
// use bidirectionalS for DAG so that in_edges() can be used.
// direction of DAG is determined by (source, target) when an edge is added to the graph
typedef adjacency_list<vecS, vecS, bidirectionalS, VertexShortestPathCount, EdgeWeight> DAG;

using Vertex = Graph::vertex_descriptor;
using EdgeData = Graph::edge_descriptor;

/**
 * @brief The GVGraph class implements Girvan-Newman algorithm using Boost Graphic Library
 */
class GVGraph {
    /**
     * @brief The Visitor struct DAG of shortest path from the root using BFS
     */
    struct Visitor : public boost::default_bfs_visitor
    {
    public:
        DAG &dag; // DAG that will be created by the BFS visitor
        int rootV; // starting vertex of the DAG
        map<int, set<Vertex>> &levelMap; // stores vertices by levels of the DAG
        /**
         * @brief constructor
         * @param iDag the DAG with shortest paths to be created
         * @param rootNode starting vertex of the DAG
         * @param iMap map of vertices at each level of the DAG
         */
        Visitor(DAG &iDag, int rootNode, map<int, set<Vertex>> &iMap) : dag(iDag), rootV(rootNode), levelMap(iMap) {}

        /**
         * @brief black_target is called when a vertex has been visited from its parent
         * and the edge is about to dequeued. This edge is part of the shortest path.
         * @param e edge of a BFS visit
         * @param g graph of the BFS
         */
        void black_target(EdgeData e, const Graph &g)
        {
            Vertex s = boost::target(e, g);
            Vertex t = boost::source(e, g);
            add_edge(s, t, 0.0, this->dag);
            int level = 0;
            if (s == rootV)
            {
                level = 1;
                // for target of root, give 1 for shortest path count to the vertex
                boost::put(vertex_rank_t(), this->dag, t, 1);
                boost::put(vertex_rank_t(), this->dag, s, 1);
            }
            else
            {
                // initialize for shortest path count to the vertex
                int spc = boost::get(vertex_rank_t(), this->dag, t);
                if(spc == 0){
                    boost::put(vertex_rank_t(), this->dag, t, 0);
                }
                for (auto const &pair : levelMap)
                {
                    level = pair.first;
                    auto vList = pair.second;
                    if (std::find_if(vList.begin(), vList.end(), [s](const int v) { return v == s; }) != vList.end())
                    {
                        ++level;
                        break;
                    }
                }
            }
            if (levelMap.find(level) == levelMap.end())
            {
                levelMap[level] = set<Vertex>();
            }
            int deleteLevel = -1;
            for (auto const &pair : levelMap)
            {
                // calculate
                auto vList = pair.second;
                // if t is in a existing level
                if (std::find_if(vList.begin(), vList.end(), [t](const int v) { return v == t; }) != vList.end())
                {
                    if (pair.first > level)
                    { // t already exist in high level set, we need to delete it
                        deleteLevel = pair.first;
                        break;
                    }
                    else
                    {
                        return;
                    }
                }
            }
            if (deleteLevel > 0)
            {
                levelMap[deleteLevel].erase(t);
            }
            levelMap[level].insert(t);
        }
    };

private:
    Graph g; //Represents the graph of the input connection data
    std::map<std::string, Vertex> indexes; //key: text name of the vertex, value: number index of the vertex
    std::map<Vertex, std::string> lookup; //key: number index of the vertex, value: text name of the vertex
    float _m; // number of edges in g
    std::map<float, set<pair<int, int>>, std::greater<float>> btw_map; // the betweeness of edges in decending order

public:
    /**
     * @brief construct a graph with a list of social network connection edges
     * @param edgeList vector of edges of pair<int, int>
     */
    GVGraph(vector<Edge> edgeList);
    /**
     * @brief aggregate betweeness of DAGs back to g
     * @return graph with aggregated betweeness on edges
     */
    Graph computeBetweeness();
    /**
     * @brief create a DAG with shortest paths to all nodes from a start vertex using BFS
     * @param start starting vertex of the DAG
     * @return a DAG
     */
    DAG computeDAG(Vertex start);
    /**
     * @brief calculate modularity (Q) of a graup of clusters
     * @param clusters
     * @return the Q (range from -1 to 1)
     */
    float computeModularity(map<int, set<int>> clusters);
    /**
     * @brief 1. remove edges with highest betweeness
     *        2. if removal creates new clusters, calculate modularity of the clusters
     *        3. track the graph with highest modularity
     *        4. stop if there are no more edges in g, otherwise repeat 1
     * @return the graph of sub-clusters with highest modularity identified in the process
     */
    pair<Graph, float> detectCommunities();

    ~GVGraph() {}
    Graph getGraph() { return g; } //return the original graph
    string node_name(int vertexIndex) { return lookup[vertexIndex]; }
    /**
     * @brief convert map<key, value> to map<value, vector<key>>
     * @param subClusters map<key, value>
     * @return map<value, vector<key>>
     */
    map<int, set<int>> convertMap(map<int, int> subClusters) {
        map<int, set<int>> clusterMap = map<int, set<int>>();
        for (auto const &[vertex, id] : subClusters) {
            if (clusterMap.find(id) == clusterMap.end()) {
                clusterMap[id] = set<int>();
            }
            clusterMap[id].insert(vertex);
        }
        return clusterMap;
    };
};


