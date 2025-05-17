// Given an undirected graph represented by the array edges[][], where each edges[i] contains three integers [u, v, w],
// representing an undirected edge from u to v, having distance w. Your task is to find the distance of the second best
// minimum spanning tree of the given graph.

#include <bits/stdc++.h>
using namespace std;

class DSU
{
    vector<int> parent, rank;

public:
    DSU(int n)
    {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++)
        {
            parent[i] = i;
            rank[i] = 1;
        }
    }

    int find(int x)
    {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    }

    void unite(int u, int v)
    {
        int root_u = find(u);
        int root_v = find(v);
        if (root_u != root_v)
        {
            if (rank[root_u] < rank[root_v])
            {
                parent[root_u] = root_v;
            }
            else if (rank[root_u] > rank[root_v])
            {
                parent[root_v] = root_u;
            }
            else
            {
                parent[root_v] = root_u;
                rank[root_u]++;
            }
        }
    }
};

bool compareEdges(vector<int> &a, vector<int> &b)
{
    return a[2] < b[2];
}

int secondMST(int V, vector<vector<int>> &edges)
{
    sort(edges.begin(), edges.end(), compareEdges);

    DSU dsu(V);
    int totalCost = 0, edgeCount = 0;
    vector<int> mstIndices;

    for (int i = 0; i < edges.size(); i++)
    {
        int u = edges[i][0], v = edges[i][1], w = edges[i][2];
        if (dsu.find(u) != dsu.find(v))
        {
            dsu.unite(u, v);
            totalCost += w;
            mstIndices.push_back(i);
            if (++edgeCount == V - 1)
                break;
        }
    }

    int secondMinCost = INT_MAX;

    for (int i = 0; i < mstIndices.size(); i++)
    {
        DSU tempDSU(V);
        int newCost = 0, newEdgeCount = 0;
        for (int j = 0; j < edges.size(); j++)
        {
            if (j == mstIndices[i])
                continue;
            int u = edges[j][0], v = edges[j][1], w = edges[j][2];
            if (tempDSU.find(u) != tempDSU.find(v))
            {
                tempDSU.unite(u, v);
                newCost += w;
                if (++newEdgeCount == V - 1)
                    break;
            }
        }
        if (newEdgeCount == V - 1)
            secondMinCost = min(secondMinCost, newCost);
    }

    return secondMinCost;
}

int main()
{
    vector<vector<int>> graph = {
        {0, 1, 18}, {0, 6, 2}, {0, 5, 8}, {0, 4, 10}, {1, 6, 12}, {1, 2, 4}, {2, 6, 14}, {2, 3, 26}, {3, 4, 16}, {3, 6, 30}, {4, 5, 5}, {4, 6, 3}};

    cout << secondMST(7, graph) << endl;

    return 0;
}