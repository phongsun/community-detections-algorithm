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

    SECTION("Test betweenness of edges"){
        cout << "Testing betweenness of graphs...";
    }
}