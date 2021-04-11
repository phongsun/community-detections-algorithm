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
    cout << num_vertices(g) << " vertices " << endl;
    float modularity = gvGraph.doAlgo();

    g = gvGraph.getGraph();
    map<int, int> subClusters;
    int nclusters = connected_components(g, make_assoc_property_map(subClusters));
    cout << nclusters << " communities detected!" << endl;
    cout << "modularity = " << modularity << endl;
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
            exit(1);
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
        exit(1);
    }
    return edgeList;
}