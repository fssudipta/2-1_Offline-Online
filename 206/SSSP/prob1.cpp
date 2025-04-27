// Given an undirected connected graph of n vertices
// and a list of m edges in a graph and
// for each pair of vertices that are connected by an edge.
// There are two edges between them, one curved edge and one straight edge
// i.e. the tuple (x, y, w1, w2) means that between vertices x and y,
// there is a straight edge with weight w1 and a curved edge with weight w2.
// You are given two vertices a and b and
// you have to go from a to b through a series of edges
// such that in the entire path you can use at most 1 curved edge.
// Your task is to find the shortest path from a to b satisfying the above condition.
// If there is no path from a to b, return -1.

#include <bits/stdc++.h>
using namespace std;

// edike pq te first thake distance ar second thake vertex, taile min heap maintain hoy
// ar adj te to first e thake neighbor vertex and then weight

vector<int> dijkstra(int u, int b, int n, vector<pair<int, int>> adj[])
{
    vector<int> dis;
    dis.assign(n + 1, 1e8);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dis[u] = 0;
    pq.push({0, u});
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        for (auto p : adj[u])
        {
            int v = p.first;
            int w = p.second;
            if (dis[v] > dis[u] + w)
            {
                dis[v] = dis[u] + w;
                pq.push({dis[v], v});
            }
        }
    }
    return dis;
}

int shortestPath(int n, int m, int a, int b, vector<vector<int>> &edges)
{
    vector<pair<int, int>> adj[n + 1];
    vector<vector<int>> curved;
    for (int i = 0; i < m; i++)
    {
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];
        int cw = edges[i][3];
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        curved.push_back({u, v, cw});
    }
    vector<int> da = dijkstra(a, b, n, adj);
    vector<int> db = dijkstra(b, a, n, adj);
    int ans = da[b];

    for (int i = 0; i < m; i++)
    {
        int u = curved[i][0];
        int v = curved[i][1];
        int cw = curved[i][2];
        ans = min(ans, da[u] + cw + db[v]);
        ans = min(ans, da[v] + cw + db[u]);
    }
    if (ans >= 1e8)
        return -1;
    return ans;
}

int main()
{

    int n = 4, m = 4;
    int a = 2, b = 4;
    vector<vector<int>> edges = {{1, 2, 1, 4},
                                 {1, 3, 2, 4},
                                 {1, 4, 3, 1},
                                 {2, 4, 6, 5}};

    // Function Call
    int ans = shortestPath(n, m, a, b, edges);
    cout << ans;

    return 0;
}