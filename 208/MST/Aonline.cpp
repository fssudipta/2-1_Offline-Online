// Problem Statement
// A power grid planner is designing a network to connect N cities using M possible bidirectional transmission lines. Each line has a positive cost.

// To ensure redundancy and reliability, the planner wants two different connection plans:

// A primary plan that connects all cities with the minimum possible product of edge costs.
// A backup plan that:
// Also connects all cities (forms a spanning tree),
// Has a product of edge costs strictly greater than the primary plan,
// Is the smallest such product among all remaining valid spanning trees.
// Your task is to compute the product of edge weights for this backup network configuration.

// If no such plan exists, print -1.

#include <bits/stdc++.h>
using namespace std;

class DSU
{
    vector<int> parent;

public:
    DSU(int n)
    {
        parent.resize(n);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int x)
    {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    bool unite(int u, int v)
    {
        int pu = find(u), pv = find(v);
        if (pu == pv)
            return false;
        parent[pu] = pv;
        return true;
    }
};

// Function to compute MST with minimum product of edge costs
double minProductMST(int V, vector<vector<pair<int, double>>> &adj,
                     vector<vector<double>> &edges, vector<int> &mstEdges)
{
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    vector<bool> inMST(V, false);
    pq.push({0.0, 0});
    double totalLog = 0;
    vector<int> parent(V, -1);

    while (!pq.empty())
    {
        auto [logWt, u] = pq.top();
        pq.pop();
        if (inMST[u])
            continue;
        inMST[u] = true;
        totalLog += logWt;

        if (parent[u] != -1)
            mstEdges.push_back(parent[u]);

        for (auto &[v, w] : adj[u])
        {
            if (!inMST[v])
                pq.push({w, v});
        }
    }

    if (count(inMST.begin(), inMST.end(), true) != V)
    {
        // Graph not connected
        return -1;
    }

    return exp(totalLog);
}

// Main function to find backup MST
double backupMST(int V, vector<vector<pair<int, double>>> &adjOriginal,
                 vector<vector<double>> &edgesOriginal)
{
    // Build adjacency list
    vector<vector<pair<int, double>>> adj = adjOriginal;
    vector<vector<double>> edges = edgesOriginal;

    vector<int> mstEdges;
    double primaryCost = -1;

    {
        // Build adjacency list again for MST computation
        vector<vector<pair<int, double>>> tmpAdj(V);
        for (auto &e : edges)
        {
            int u = e[0], v = e[1];
            double w = e[2];
            tmpAdj[u].push_back({v, log(w)});
            tmpAdj[v].push_back({u, log(w)});
        }

        primaryCost = minProductMST(V, tmpAdj, edges, mstEdges);
        if (primaryCost == -1)
            return -1;
    }

    double backupCost = INFINITY;

    for (int skipIdx : mstEdges)
    {
        double currCost = 0;
        DSU dsu(V);
        int countEdges = 0;
        vector<vector<pair<int, double>>> tmpAdj = adj;

        for (int j = 0; j < edges.size(); ++j)
        {
            if (j == skipIdx)
                continue;
            int u = edges[j][0], v = edges[j][1];
            double w = edges[j][2];

            if (dsu.unite(u, v))
            {
                currCost += log(w);
                if (++countEdges == V - 1)
                    break;
            }
        }

        if (countEdges == V - 1)
        {
            backupCost = min(backupCost, exp(currCost));
        }
    }

    if (backupCost > primaryCost)
        return backupCost;
    else
        return -1;
}

int main()
{
    int N = 4;
    vector<vector<double>> edges = {
        {0, 1, 2}, {0, 2, 3}, {1, 2, 4}, {1, 3, 5}, {2, 3, 6}};

    // Build adjacency list with log weights
    vector<vector<pair<int, double>>> adj(N);
    for (auto &e : edges)
    {
        int u = e[0], v = e[1];
        double w = e[2];
        adj[u].push_back({v, log(w)});
        adj[v].push_back({u, log(w)});
    }

    double result = backupMST(N, adj, edges);
    if (result == -1)
    {
        cout << "-1\n";
    }
    else
    {
        cout << fixed << setprecision(6) << result << "\n";
    }

    return 0;
}