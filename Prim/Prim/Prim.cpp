#include "prim.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <limits>
#include <stdexcept>

using namespace std;

bool tryLoadGraph(const string& filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file \"" << filename << "\"\n";
        return false;
    }

    if (!(file >> graph.numVertices) || graph.numVertices <= 0) {
        cerr << "Error: Invalid number of vertices.\n";
        return false;
    }

    if (!(file >> graph.numEdges) || graph.numEdges < 0) {
        cerr << "Error: Invalid number of edges.\n";
        return false;
    }

    graph.adjList.assign(graph.numVertices, {});

    for (int i = 0; i < graph.numEdges; ++i) {
        Edge e;
        if (!(file >> e.u >> e.v >> e.weight)) {
            cerr << "Error: Could not read edge " << i + 1 << ".\n";
            return false;
        }

        if (e.u < 0 || e.u >= graph.numVertices ||
            e.v < 0 || e.v >= graph.numVertices) {
            cerr << "Error: Edge " << i + 1
                << " references out-of-range vertex.\n";
            return false;
        }

        graph.edges.push_back(e);
        graph.adjList[e.u].push_back({ e.v, e.weight });
        graph.adjList[e.v].push_back({ e.u, e.weight }); // undirected
    }

    return true;
}


bool tryGetPrimsMST(const Graph& graph, vector<Edge>& mstEdges, int& totalWeight) {
    const int V = graph.numVertices;
    if (V == 0) return false;

    vector<int> key(V, numeric_limits<int>::max());
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);

    using PQEntry = pair<int, int>;
    priority_queue<PQEntry, vector<PQEntry>, greater<PQEntry>> pq;

    key[0] = 0;
    pq.push({ 0, 0 });

    totalWeight = 0;
    mstEdges.clear();

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (inMST[u]) continue; 
        inMST[u] = true;

        if (parent[u] != -1) {
            mstEdges.push_back({ parent[u], u, key[u] });
            totalWeight += key[u];
        }

        for (const pair<int, int>& neighbor : graph.adjList[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push({ key[v], v });
            }
        }
    }

    for (int v = 0; v < V; ++v) {
        if (!inMST[v]) {
            cerr << "Error: Graph is disconnected — no spanning tree exists.\n";
            return false;
        }
    }

    return true;
}


void printMST(const vector<Edge>& mstEdges, int totalWeight) {
    cout << "Minimum Spanning Tree edges:\n";
    for (const Edge& e : mstEdges) {
        cout << "   " << e.u << " -- " << e.v
            << "  :  " << e.weight << "\n";
    }
    cout << "Total MST weight: " << totalWeight << "\n";
}


int main(int argc, char* argv[]) {
    string filename = "input.txt";

    Graph graph;
    tryLoadGraph(filename, graph);

    vector<Edge> mstEdges;
    int totalWeight = 0;
    tryGetPrimsMST(graph, mstEdges, totalWeight);

    printMST(mstEdges, totalWeight);
    return 0;
}