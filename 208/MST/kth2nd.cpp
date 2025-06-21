#include <bits/stdc++.h>
using namespace std;

class DSU
{
    vector<int> parent;

public:
    DSU(int n)
    {
        parent.resize(n);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x)
    {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    bool unite(int u, int v)
    {
        int root_u = find(u), root_v = find(v);
        if (root_u == root_v)
            return false;
        parent[root_v] = root_u;
        return true;
    }
};

// Global variables
static int countMST = 0;
static int resultCost = -1;
set<vector<bool>> generated;

// Build MST using Kruskal's algorithm, skipping one specific edge
int buildMST(int V, const vector<vector<int>> &edges, int skipIndex)
{
    DSU dsu(V);
    int totalCost = 0, edgeCount = 0;

    for (int i = 0; i < edges.size(); i++)
    {
        if (i == skipIndex)
            continue;
        int u = edges[i][0], v = edges[i][1], w = edges[i][2];
        if (dsu.unite(u, v))
        {
            totalCost += w;
            edgeCount++;
            if (edgeCount == V - 1)
                break;
        }
    }

    if (edgeCount == V - 1)
        return totalCost;
    else
        return -1; // Not connected
}

// Recursive function to generate all possible MSTs
void kthMST(int V, const vector<vector<int>> &edges, int K, int skipIndex)
{
    if (resultCost != -1)
        return; // Already found K-th MST

    int mstCost = buildMST(V, edges, skipIndex);

    if (mstCost != -1)
    {
        vector<bool> used(edges.size(), false);
        DSU dsu(V);
        int edgeCount = 0;

        // Create a signature of this MST to avoid duplicates
        vector<bool> signature(edges.size(), false);
        for (int i = 0; i < edges.size(); i++)
        {
            if (i == skipIndex)
                continue;
            int u = edges[i][0], v = edges[i][1];
            if (dsu.unite(u, v))
            {
                signature[i] = true;
                if (++edgeCount == V - 1)
                    break;
            }
        }

        if (generated.find(signature) == generated.end())
        {
            generated.insert(signature);
            countMST++;

            if (countMST == K)
            {
                resultCost = mstCost;
                return;
            }
        }
    }

    // Try removing another edge
    for (int i = 0; i < edges.size(); i++)
    {
        if (i != skipIndex)
        {
            kthMST(V, edges, K, i);
        }
    }
}

// Driver code
int main()
{
    vector<vector<int>> graph = {
        {0, 1, 18}, {0, 6, 2}, {0, 5, 8}, {0, 4, 10}, {1, 6, 12}, {1, 2, 4}, {2, 6, 14}, {2, 3, 26}, {3, 4, 16}, {3, 6, 30}, {4, 5, 5}, {4, 6, 3}};

    int V = 7; // Number of vertices
    int K = 2; // Find second MST

    kthMST(V, graph, K, -1); // -1 means no edge skipped initially

    if (resultCost != -1)
        cout << "The " << K << "-th smallest MST has total weight: " << resultCost << endl;
    else
        cout << "Could not find the " << K << "-th MST." << endl;

    return 0;
}