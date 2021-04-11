/**
 * @brief source file for the main function of the Girvan-Newman Algorithm implementation
 * @author Peter Sun
 * @date 4-5-2021
 * @version 1.0
 */
#include "GVGraph.h"
//#define DEBUG 1
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
    GVGraph girvan_newman = GVGraph(edgeList);
#ifdef DEBUG
    Graph g = girvan_newman.getGraph();
    cout << num_vertices(g) << " vertices " << endl;
#endif
    pair<Graph, float> gvResult = girvan_newman.detectCommunities();
    float modularity = gvResult.second;
    Graph communities = gvResult.first;;

    map<int, int> subClusters;
    int nclusters = connected_components(communities, make_assoc_property_map(subClusters));
    cout << nclusters << " communities detected!";
    cout << " - modularity = " << modularity << endl;
    map<int, set<int>> communityMap = girvan_newman.convertMap(subClusters);
    for (auto c: communityMap) {
        cout << "Community " << c.first << endl;
        for (auto node: c.second) {
            cout << girvan_newman.node_name(node) << endl;
        }
    }
}

vector<Edge> loadInputFile(string fileName)
{
    string line;
    ifstream graphFile(fileName);
    vector<Edge> edgeList;

    if (graphFile.is_open())
    {
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
        exit(1);
    }
    return edgeList;
}