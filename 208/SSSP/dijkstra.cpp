#include <bits/stdc++.h>
using namespace std;

vector<vector<vector<int>>> constructAdj(vector<vector<int>> &edges, int V)
{
    vector<vector<vector<int>>> adj(V);
    for (const auto &edge : edges)
    {
        int u = edge[0], v = edge[1], w = edge[2];
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // For undirected graph
    }
    return adj;
}

vector<int> dijkstra(int V, vector<vector<int>> &edges, int src)
{
    vector<vector<vector<int>>> adj = constructAdj(edges, V);
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
    pq.push({0, src}); // {distance, vertex}
    while (!pq.empty())
    {
        int d = pq.top()[0], u = pq.top()[1];
        pq.pop();
        if (d > dist[u])
            continue; // Skip if we have already found a better path
        for (const auto &neighbor : adj[u])
        {
            int v = neighbor[0], w = neighbor[1];
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

int main()
{
    int v = 5;
    int src = 0;
    vector<vector<int>> edges = {{0, 1, 4}, {0, 2, 8}, {1, 4, 6}, {2, 3, 2}, {3, 4, 10}};
    vector<int> res = dijkstra(v, edges, src);
    for (int dist : res)
    {
        cout << dist << " ";
    }
    return 0;
}