#include <bits/stdc++.h>
using namespace std;

#define V 6

bool BFS(int rgraph[V][V], int s, int t, int p[])
{
    bool visited[V];
    memset(visited, 0, sizeof(visited));
    queue<int> q;
    q.push(s);
    visited[s] = true;
    p[s] = -1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v = 0; v < V; v++)
        {
            if (visited[v] == false && rgraph[u][v] > 0)
            {
                if (v == t)
                {
                    p[v] = u;
                    return true;
                }
                q.push(v);
                p[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

int fordFulkerson(int graph[V][V], int s, int t)
{
    int u, v;
    int rGraph[V][V];
    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];
    int p[V];
    int maxflow = 0;
    while (BFS(rGraph, s, t, p))
    {
        int path = INT_MAX;
        for (v = t; v != s; v = p[v])
        {
            u = p[v];
            rGraph[u][v] -= path;
            rGraph[v][u] += path;
        }
        maxflow += path;
    }
    return maxflow;
}

int main()
{
    int graph[V][V] = {{0, 16, 13, 0, 0, 0}, {0, 0, 10, 12, 0, 0}, {0, 4, 0, 0, 14, 0}, {0, 0, 9, 0, 0, 20}, {0, 0, 0, 7, 0, 4}, {0, 0, 0, 0, 0, 0}};
    cout << "The max pawssible flow is: " << fordFulkerson(graph, 0, 5);
    return 0;
}
