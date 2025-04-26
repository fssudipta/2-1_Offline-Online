#include <bits/stdc++.h>
using namespace std;

void addEdge(vector<vector<int>>&adj, int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u); // For undirected graph
}

void DFSUtil(int v, vector<bool> &visited, vector<vector<int>> &adj) {
    visited[v] = true;
    cout << v << " ";

    for (int i : adj[v]) {
        if (!visited[i]) {
            DFSUtil(i, visited, adj);
        }
    }
}

vector <int> DFS(vector<vector<int>> &adj) {
    int n = adj.size();
    vector<int> result;
    vector<bool> visited(n, false);

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFSUtil(i, visited, adj);
        }
    }

    return result;
}

int main() {
    int n = 5; // Number of vertices
    vector<vector<int>> adj(n);

    addEdge(adj, 0, 1);
    addEdge(adj, 0, 2);
    addEdge(adj, 1, 3);
    addEdge(adj, 1, 4);

    cout << "Depth First Traversal starting from vertex 0:\n";
    DFS(adj);

    return 0;
}