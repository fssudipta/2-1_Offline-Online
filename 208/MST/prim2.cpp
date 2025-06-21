#include <bits/stdc++.h>
using namespace std;

int spanningTree(int V, int E, vector<vector<int>> &edges)
{
    vector<vector<int>> adj[V];

    for (int i = 0; i < E; i++)
    {
        int u = edges[i][0], v = edges[i][1], wt = edges[i][2];
        adj[u].push_back({v, wt});
        adj[v].push_back({u, wt});
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<bool> visited(V, false);
    int res = 0;

    pq.push({0, 0});

    while (!pq.empty())
    {
        auto p = pq.top();
        pq.pop();

        int wt = p.first;
        int u = p.second;

        if (visited[u])
            continue;

        res += wt;
        visited[u] = true;

        for (auto v : adj[u])
        {
            //v[0] mane vertex ar v[1] mane wt
            if (!visited[v[0]])
            {
                pq.push({v[1], v[0]});
            }
        }
    }

    return res;
}

int main()
{
    vector<vector<int>> graph = {{0, 1, 5}, {1, 2, 3}, {0, 2, 1}};
    cout << spanningTree(3, 3, graph) << endl;
    return 0;
}