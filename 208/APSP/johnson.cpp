#include <bits/stdc++.h>
using namespace std;

bool bellmanford(int V, const vector<vector<int>> &edges, vector<int> &dist)
{
    dist.assign(V, INT_MAX);
    dist[V - 1] = 0;
    for (int i = 0; i < V; i++)
    {
        for (const auto &edge : edges)
        {
            int u = edge[0], v = edge[1], weight = edge[2];
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
        for (const auto &edge : edges)
        {
            int u = edge[0], v = edge[1], weight = edge[2];
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                return false;
        }
    }
    return true;
}

void dijkstra(int src, int V, const vector<vector<pair<int, int>>> &adj, vector<int> &dist)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist.assign(V, INT_MAX);
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty())
    {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();
        if (d > dist[u])
            continue;
        for (const auto &edge : adj[u])
        {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
}

void johnson(int V, const vector<vector<int>> &edges)
{
    vector<vector<int>> modEdges = edges;
    for (int i = 0; i < V; i++)
    {
        modEdges.push_back({V, i, 0}); // new vertex connected to all vertex
    }
    // bellman ford lessgoo
    vector<int> h(V + 1, 0); // new potential for the new vertex
    if (!bellmanford(V + 1, modEdges, h))
    {
        cout << "Graph contains a negative weight cycle" << endl;
        return;
    }
    // rewrite the edges
    vector<vector<pair<int, int>>> adj(V);
    for (const auto &edge : edges)
    {
        int u = edge[0], v = edge[1], weight = edge[2];
        adj[u].emplace_back(v, weight + h[u] - h[v]);
    }
    // dijkstra's algorithm
    vector<vector<int>> shortestPath(V, vector<int>(V, 0));
    for (int i = 0; i < V; i++)
    {
        vector<int> dist;
        dijkstra(i, V, adj, dist);
        // convert distances back to original weights
        for (int j = 0; j < V; j++)
        {
            if (dist[j] == INT_MAX)
                shortestPath[i][j] = INT_MAX; // no path exists
            else
                shortestPath[i][j] = dist[j] - h[i] + h[j];
        }
    }
    cout << "shortest paths between all parts of vertices: " << endl;
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (shortestPath[i][j] == INT_MAX)
                cout << "INF" << endl;
            else
                cout << shortestPath[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int V, E;
    cout << "Enter the number of vertices and edges: ";
    cin >> V >> E;
    vector<vector<int>> edges(E, vector<int>(3));
    for (int i = 0; i < E; i++)
    {
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
    }
    johnson(V, edges);
    return 0;
}

/* Enter the number of vertices and edges: 4 5
Enter the edges (u, v, weight):
0 1 3
0 2 8
1 2 -4
2 3 7
3 0 2 */