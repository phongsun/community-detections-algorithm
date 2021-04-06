/**
 * @brief source file for the main function of the Girvan-Newman Algorithm implementation
 * @author Peter Sun
 * @date 4-5-2021
 * @version 1.0
 */
#include "GVGraph.h"
// Declare function for loading input file
vector<Edge> loadInputFile(string fileName);
/**
 * @brief Load file of the social network connections, create a GVGraph object, and then
 * compute community clusters in the GVGraph object
 * @return 0 if successful, 1 if there's an error
 */
int main()
{
    //Load file of social network connections into a vector<pair<string, string>>
    vector<Edge> edgeList = loadInputFile("data/put_data_here.txt");
    //Create a GVGraph object of a boost BGL undirected adjacency list
    GVGraph gvGraph = GVGraph(edgeList);
    Graph g = gvGraph.getGraph();

    std::cout << "edges(g) = ";
    graph_traits<Graph>::edge_iterator ei, ei_end;
    int j = 0;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        std::cout << "(" << source(*ei, g)
                  << "," << target(*ei, g) << ") ";
        j++;
    }
    std::cout << std::endl << j << endl;
}

vector<Edge> loadInputFile(string fileName)
{
    string line;
    ifstream graphFile(fileName);
    vector<Edge> edgeList;

    if (graphFile.is_open())
    {
        // Check if the first line is '''text
        getline(graphFile, line);
        boost::trim(line);
        if (line != "'''text")
        {
            cout << "First line of file is not right" << endl;
        }
        // Get number of edges
        getline(graphFile, line);
        boost::trim(line);
        int numOfEdges = stoi(line);
        for (int i = 0; i < numOfEdges; i++)
        {
            getline(graphFile, line);
            boost::trim(line);
            vector<string> tmp;
            boost::split(tmp, line, boost::is_any_of("-"));
            boost::trim(tmp[0]);
            boost::trim(tmp[1]);
            Edge edge = Edge(tmp[0], tmp[1]);
            edgeList.push_back(edge);
        }
    }
    else
    {
        cout << "Fail to open the file." << endl;
    }
    return edgeList;
}

/*
Graph loadInputFile(string fileName)
{
    string line;
    ifstream graphFile(fileName);

    if (graphFile.is_open())
    {
        // Check if the first line is '''text
        getline(graphFile, line);
        boost::trim(line);
        if (line != "'''text")
        {
            cout << "First line is not right" << endl;
        }
        // Get number of edges
        getline(graphFile, line);
        boost::trim(line);
        int numOfEdges = stoi(line);
        vector<Edge> edgeList;
        set<string> vertices;
        for (int i = 0; i < numOfEdges; i++)
        {
            getline(graphFile, line);
            boost::trim(line);
            vector<string> tmp;
            boost::split(tmp, line, boost::is_any_of("-"));
            boost::trim(tmp[0]);
            boost::trim(tmp[1]);
            Edge edge = Edge(tmp[0], tmp[1]);
            edgeList.push_back(edge);
            vertices.insert(tmp[0]);
            vertices.insert(tmp[1]);
        }
        cout << vertices.size() << endl;
        Graph g(vertices.size());

        // fills the property 'vertex_name_t' of the vertices
        set<string>:: iterator it;
        int k = 0;
        for(it = vertices.begin(); it!=vertices.end(); ++it, k++){
            string v = *it;
            boost::put(vertex_name_t(), g, k, v); // set the property of a vertex
            indexes[v] = boost::vertex(k, g);     // retrives the associated vertex descriptor
            lookup[boost::vertex(k, g)] = v;
        }

        for (int i = 0; i < edgeList.size(); ++i){
            add_edge(indexes[edgeList[i].first], indexes[edgeList[i].second], g);
        }
        graphFile.close();

        Vertex start = 0;
        Visitor visitor;

         //boost::breadth_first_search(g, start, boost::visitor(visitor));

        std::cout << "edges(g) = ";
        graph_traits<Graph>::edge_iterator ei, ei_end;
        int j = 0;
        for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
            std::cout << "(" << lookup[source(*ei, g)]
                    << "," << lookup[target(*ei, g)] << ") ";
                    j++;
        }
        std::cout << std::endl << j << endl;
        std::cout << numOfEdges;


         return g;
    }
    else
    {
        cout << "ahhh something's wrong" << endl;
    }
    return NULL;
}
*/