// Minimize count of connections required to be rearranged to make all the computers connected
#include <bits/stdc++.h>
using namespace std;

void DFS(unordered_map<int, vector<int>> &adj, int node, vector<bool> &visited)
{
    if (visited[node])
        return;
    visited[node] = true;
    for (auto x : adj[node])
    {
        if (!visited[x])
            DFS(adj, x, visited);
    }
}

int makeConnectedUtil(int N, int connections[][2], int M)
{
    vector<bool> visited(N, false);
    unordered_map<int, vector<int>> adj;
    int edges = 0;

    for (int i = 0; i < M; ++i)
    {
        adj[connections[i][0]].push_back(connections[i][1]);
        adj[connections[i][1]].push_back(connections[i][0]);
        edges += 1;
    }

    int components = 0;
    for (int i = 0; i < N; ++i)
    {
        if (!visited[i])
        {
            components += 1;
            DFS(adj, i, visited);
        }
    }

    if (edges < N - 1)
        return -1;

    int redundant = edges - ((N - 1) - (components - 1));

    if (redundant >= (components - 1))
        return components - 1;

    return -1;
}

void makeConnected(int N, int connections[][2], int M)
{
    int minOps = makeConnectedUtil(N, connections, M);
    cout << minOps;
}

int main()
{
    int N = 4;
    int connections[][2] = {{0, 1}, {0, 2}, {1, 2}};
    int M = sizeof(connections) / sizeof(connections[0]);
    makeConnected(N, connections, M);
    return 0;
}