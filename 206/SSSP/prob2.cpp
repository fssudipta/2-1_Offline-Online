// Given a Weighted Directed Acyclic Graph and a source vertex in the graph,
// find the shortest paths from given source to all other vertices.

#include <bits/stdc++.h>
using namespace std;

void topologicalSortUtil(int u, vector<bool> &visited, stack<int> &stack, vector<pair<int, int>> adj[])
{
    visited[u] = true;
    for (auto edge : adj[u])
    {
        int v = edge.first;
        if (!visited[v])
        {
            topologicalSortUtil(v, visited, stack, adj);
        }
    }
    stack.push(u);
}

vector<int> shortestPathInDAG(int V, vector<pair<int, int>> adj[], int src)
{
    stack<int> topoStack;
    vector<bool> visited(V, false);
    for (int i = 0; i < V; i++)
    {
        if (!visited[i])
            topologicalSortUtil(i, visited, topoStack, adj);
    }
    vector<int> dist(V, 1e8);
    dist[src] = 0;
    while (!topoStack.empty())
    {
        int u = topoStack.top();
        topoStack.pop();
        if (dist[u] != 1e8)
        {
            for (auto edge : adj[u])
            {
                int v = edge.first;
                int wt = edge.second;
                if (dist[v] > dist[u] + wt)
                    dist[v] = dist[u] + wt;
            }
        }
    }
}

int main()
{
    int V = 6;                     // Number of vertices
    vector<pair<int, int>> adj[V]; // Adjacency list

    // Adding edges with weights
    adj[0].push_back({1, 5});
    adj[0].push_back({2, 3});
    adj[1].push_back({3, 6});
    adj[1].push_back({2, 2});
    adj[2].push_back({4, 4});
    adj[2].push_back({5, 2});
    adj[2].push_back({3, 7});
    adj[3].push_back({4, -1});
    adj[4].push_back({5, -2});

    int src = 0; // Source vertex
    // Find shortest paths
    vector<int> dist = shortestPathInDAG(V, adj, src);

    // Print shortest distances
    cout << "Shortest distances from source vertex " << src << ":\n";
    for (int i = 0; i < V; i++)
    {
        if (dist[i] == 1e9)
        {
            cout << "Vertex " << i << ": INF\n";
        }
        else
        {
            cout << "Vertex " << i << ": " << dist[i] << "\n";
        }
    }

    return 0;
}