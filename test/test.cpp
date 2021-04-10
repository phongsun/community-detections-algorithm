#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/GVGraph.h"
TEST_CASE( "Test GVGraph", "[data]") {
    enum
    {
        A, B, C, D, E, F, G, H, I, J, K, N
    };
    cout << "Testing GVGraph...";

    vector<Edge> edgeList = {
            Edge("A", "B"),
            Edge("A", "C"),
            Edge("A", "D"),
            Edge("A", "E"),
            Edge("B", "F"),
            Edge("B", "C"),
            Edge("C", "F"),
            Edge("D", "G"),
            Edge("D", "H"),
            Edge("E", "H"),
            Edge("F", "I"),
            Edge("G", "I"),
            Edge("G", "J"),
            Edge("H", "J"),
            Edge("I", "K"),
            Edge("J", "K")
    };

    GVGraph gvGraph = GVGraph(edgeList);
    DAG dag = gvGraph.computeDAG(A);


    SECTION("generation of a GVGraph") {
        cout << "Testing generation of a GVGraph" << flush;
        Graph g = gvGraph.getGraph();
        REQUIRE(boost::num_edges(g) == edgeList.size());
        cout << "...passed" << endl;
    }
    SECTION("test generation of DAG via BFS") {
        cout << "Testing generation of a DAG via BFS" << flush;
        typedef graph_traits<DAG>::out_edge_iterator out_edge_it;

        out_edge_it ei, ei_end;
        set<Vertex> level1;
        int edgeCnt = 0;
        for (boost::tie(ei, ei_end) = out_edges(0, dag); ei != ei_end; ++ei)
        {
            Vertex t = target(*ei, dag);
            bool criteria;
            criteria = (t == B || t == C || t == D || t == E);
            level1.insert(t);
            REQUIRE(criteria == true);
            edgeCnt++;
        }
        REQUIRE(edgeCnt == 4);
        REQUIRE(level1.size() == 4);

        // level 2
        set<Vertex> level2;
        for(auto v: level1){
            edgeCnt = 0;
            for (boost::tie(ei, ei_end) = out_edges(v, dag); ei != ei_end; ++ei)
            {
                Vertex t = target(*ei, dag);
                bool criteria;
                switch(v){
                    case B:
                        criteria = (t == F || t == C);
                        break;
                    case C:
                        criteria = (t == F || t == B);
                        break;
                    case D:
                        criteria = (t == G || t == H);
                        break;
                    case E:
                        criteria = (t == H);
                        break;
                }
                level2.insert(t);
                REQUIRE(criteria == true);
                edgeCnt++;
            }
            switch(v){
                case B:
                    REQUIRE(edgeCnt == 2);
                    break;
                case C:
                    REQUIRE(edgeCnt == 1);
                    break;
                case D:
                    REQUIRE(edgeCnt == 2);
                    break;
                case E:
                    REQUIRE(edgeCnt == 1);
                    break;
            }
        }

        level2.erase(C);
        REQUIRE(level2.size() == 3);
        // level 3
        set<Vertex> level3;
        for(auto v: level2){
            edgeCnt = 0;
            for (boost::tie(ei, ei_end) = out_edges(v, dag); ei != ei_end; ++ei)
            {
                Vertex t = target(*ei, dag);
                bool criteria;
                switch(v){
                    case F:
                        criteria = (t == I);
                        break;
                    case G:
                        criteria = (t == I || t == J);
                        break;
                    case H:
                        criteria = (t == J);
                        break;
                }
                level3.insert(t);
                REQUIRE(criteria == true);
                edgeCnt++;
            }
            switch(v){
                case F:
                    REQUIRE(edgeCnt == 1);
                    break;
                case G:
                    REQUIRE(edgeCnt == 2);
                    break;
                case H:
                    REQUIRE(edgeCnt == 1);
                    break;
            }
        }

        // level 4
        set<Vertex> level4;
        for(auto v: level3){
            edgeCnt = 0;
            for (boost::tie(ei, ei_end) = out_edges(v, dag); ei != ei_end; ++ei)
            {
                Vertex t = target(*ei, dag);
                bool criteria;
                switch(v){
                    case I:
                        criteria = (t == K);
                        break;
                    case J:
                        criteria = (t == K);
                        break;
                }
                level4.insert(t);
                REQUIRE(criteria == true);
                edgeCnt++;
            }
            switch(v){
                case I:
                    REQUIRE(edgeCnt == 1);
                    break;
                case J:
                    REQUIRE(edgeCnt == 1);
                    break;
            }
        }
        cout << "...passed" << endl;
    }

    SECTION("Test shortest path for each node"){
        map<int, int> shortestPathMap = map<int, int>{
                {A, 1}, {B, 1}, {C, 1},
                {D, 1}, {E, 1}, {F, 2},
                {G, 1}, {H, 2}, {I, 3},
                {J, 3}, {K, 6}
        };
        cout << "Testing shortest path for each node" << flush;
        for (auto vd : boost::make_iterator_range(boost::vertices(dag))){
            int shortestPath = get(boost::vertex_rank_t(), dag, vd);
            REQUIRE(shortestPathMap[vd] == shortestPath);
        }
        cout << "...passed" << endl;
    }

    SECTION("Test betweenness for each edge in an individual BFS DAG"){
        cout << "Testing betweenness for each edge in an individual BFS DAG" << flush;
        map<pair<int, int>, float> edgeMap = map<pair<int, int>, float>{
                {pair<int, int>(A, B), 2}, {pair<int, int>(A, C), 2}, {pair<int, int>(A, D), 4},
                {pair<int, int>(A, E), 2}, {pair<int, int>(B, F), 1}, {pair<int, int>(C, F), 1},
                {pair<int, int>(D, G), 2}, {pair<int, int>(D, H), 1}, {pair<int, int>(E, H), 1},
                {pair<int, int>(F, I), 1}, {pair<int, int>(G, I), 0.5}, {pair<int, int>(G, J), 0.5},
                {pair<int, int>(H, J), 1}, {pair<int, int>(I, K), 0.5}, {pair<int, int>(J, K), 0.5}
        };
        typedef graph_traits<DAG>::edge_iterator edge_it;
        edge_it ei, ei_end;
        for (boost::tie(ei, ei_end) = edges(dag); ei != ei_end; ++ei){
            float betweenness = get(boost::edge_weight_t(), dag, *ei);
            pair<int, int> e = pair<int, int>(source(*ei, dag), target(*ei, dag));
            REQUIRE(edgeMap[e] == betweenness);
        }
        cout << "...passed" << endl;
    }

    SECTION("Test aggregated completeness for the graph"){
        cout << "Testing aggregated completeness for the graph" << flush;
        vector<Edge> eL = {
                Edge("A", "B"),
                Edge("A", "C"),
                Edge("B", "C"),
                Edge("B", "D"),
                Edge("D", "E"),
                Edge("D", "F"),
                Edge("D", "G"),
                Edge("E", "F"),
                Edge("F", "G"),
        };
        GVGraph gvGraph1 = GVGraph(eL);
        Graph g1 = gvGraph1.computeBetweeness();
        graph_traits<Graph>::edge_iterator gei, gei_end;
        map<pair<int, int>, float> edgeMap = map<pair<int, int>, float>{
                {pair<int, int>(A, B), 5}, {pair<int, int>(A, C), 1},
                {pair<int, int>(B, C), 5}, {pair<int, int>(B, D), 12}, {pair<int, int>(D, G), 4.5},
                {pair<int, int>(D, F), 4}, {pair<int, int>(D, E), 4.5}, {pair<int, int>(E, F), 1.5},
                {pair<int, int>(F, G), 1.5}
        };
        for (boost::tie(gei, gei_end) = edges(g1); gei != gei_end; ++gei)
        {
            // find weight of current edge and divide it by 2
            float gBtw = get(edge_weight_t(), g1, *gei);
            pair<int, int> e = pair<int, int>(source(*gei, dag), target(*gei, dag));
            REQUIRE(edgeMap[e] == gBtw);
        }
        cout << "...passed" << endl;
    }
}