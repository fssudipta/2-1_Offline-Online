// Given a weighted directed graph consisting of V vertices and E edges.
// The task is to print the cyclic path whose sum of weight is negative.
// If there is no such path present then print “-1”.

#include <bits/stdc++.h>
using namespace std;
// basically bellman ford

void findNegCycle(int V, vector<vector<int>> &edges)
{
    vector<int> dist(V, 1e8);
    vector<int> parent(V, -1);
    dist[0] = 0;
    for (int i = 0; i < V - 1; i++)
    {
        for (auto &edge : edges)
        {
            int u = edge[0], v = edge[1], w = edge[2];
            if (dist[u] != 1e8 && dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
    }
    // check for neg wt cycle
    int cycle_start = -1;
    for (auto &edge : edges)
    {
        int u = edge[0], v = edge[1], w = edge[2];
        if (dist[u] != 1e8 && dist[v] > dist[u] + w)
        {
            cycle_start = v;
            break;
        }
    }
    if (cycle_start == -1)
    {
        cout << "-1" << endl;
        return;
    }
    vector<int> cycle;
    vector<bool> visited(V, false);

    for (int i = 0; i < V; i++)
    {
        cycle_start = parent[cycle_start];
    }
    int current = cycle_start;
    while (!visited[current])
    {
        visited[current] = true;
        cycle.push_back(current);
        current = parent[current];
    }
    cycle.push_back(cycle_start);
    reverse(cycle.begin(), cycle.end());
    for (auto vertex : cycle)
    {
        cout << vertex << " ";
    }
    cout << endl;
}

int main()
{
    int V, E;
    cin >> V >> E;

    // Input edges as {u, v, w} (edge from u to v with weight w)
    vector<vector<int>> edges(E, vector<int>(3));
    for (int i = 0; i < E; i++)
    {
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
    }

    // Find and print the negative weight cycle
    findNegCycle(V, edges);

    return 0;
}