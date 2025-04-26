#include <bits/stdc++.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // assuming undirected graph
    }

    void removeEdge(int u, int v) {
        adj[u].erase(remove(adj[u].begin(), adj[u].end(), v), adj[u].end());
        adj[v].erase(remove(adj[v].begin(), adj[v].end(), u), adj[v].end());
    }

    void printGraph() {
        for (int i = 0; i < V; ++i) {
            cout << i << ": ";
            for (int neighbor : adj[i]) {
                cout << neighbor << " ";
            }
            cout << "\n";
        }
    }

    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS starting from node " << start << ": ";
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            for (int neighbor : adj[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << "\n";
    }

    void DFSUtil(int node, vector<bool>& visited) {
        visited[node] = true;
        cout << node << " ";

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                DFSUtil(neighbor, visited);
            }
        }
    }

    void DFS(int start) {
        vector<bool> visited(V, false);
        cout << "DFS starting from node " << start << ": ";
        DFSUtil(start, visited);
        cout << "\n";
    }

    void checkBipartite() {
        vector<int> color(V, -1);
        queue<int> q;

        for (int start = 0; start < V; ++start) {
            if (color[start] == -1) {
                color[start] = 0;
                q.push(start);

                while (!q.empty()) {
                    int node = q.front();
                    q.pop();

                    for (int neighbor : adj[node]) {
                        if (color[neighbor] == -1) {
                            color[neighbor] = 1 - color[node];
                            q.push(neighbor);
                        } else if (color[neighbor] == color[node]) {
                            cout << "Graph is NOT bipartite.\n";
                            return;
                        }
                    }
                }
            }
        }
        cout << "Graph is bipartite.\n";
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    Graph g(V);
    cout << "Enter " << E << " edges (u v):\n";
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    g.printGraph();

    int start;
    cout << "Enter starting node for BFS and DFS: ";
    cin >> start;
    g.BFS(start);
    g.DFS(start);

    g.checkBipartite();

    return 0;
}
