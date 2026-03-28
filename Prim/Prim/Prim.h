#pragma once

#include <vector>
#include <string>

using namespace std;

struct Edge {
    int u, v, weight;
};

struct Graph {
    int numVertices;
    int numEdges;
    vector<Edge> edges;
    
    vector<vector<pair<int, int>>> adjList;
};

// Try loading the graph from file
bool tryLoadGraph(const string& filename, Graph& graph);

bool tryGetPrimsMST(const Graph& graph, vector<Edge>& mstEdges, int& totalWeight);

// Print the MST
void printMST(const vector<Edge>& mstEdges, int totalWeight);