#include <bits/stdc++.h>
using namespace std;

#define V 20

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
        int path_flow = INT_MAX;
        for (v = t; v != s; v = p[v])
        {
            u = p[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        for (v = t; v != s; v = p[v])
        {
            u = p[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        maxflow += path_flow;
    }
    return maxflow;
}

int getNodeIndex(string node, map<string, int> &nodeMap, int &nodeCount)
{
    if (nodeMap.find(node) == nodeMap.end())
    {
        nodeMap[node] = nodeCount++;
    }
    return nodeMap[node];
}

int main()
{
    int E;
    cin >> E;

    int graph[V][V];
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            graph[i][j] = 0;

    map<string, int> nodeMap;
    int nodeCount = 0;
    for (int i = 0; i < E; i++)
    {
        string u, v;
        int capacity;
        cin >> u >> v >> capacity;

        int uIndex = getNodeIndex(u, nodeMap, nodeCount);
        int vIndex = getNodeIndex(v, nodeMap, nodeCount);

        graph[uIndex][vIndex] = capacity;
    }
    int source = nodeMap["S"];
    int sink = nodeMap["T"];

    cout << fordFulkerson(graph, source, sink) << endl;

    return 0;
}