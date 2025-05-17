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
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    bool unite(int u, int v)
    {
        int root_u = find(u), root_v = find(v);
        if (root_u == root_v)
            return false;
        if (rank[root_u] < rank[root_v])
            swap(root_u, root_v);
        parent[root_v] = root_u;
        if (rank[root_u] == rank[root_v])
            rank[root_u]++;
        return true;
    }

    DSU copy()
    {
        DSU newDsu(0);
        newDsu.parent = this->parent;
        newDsu.rank = this->rank;
        return newDsu;
    }
};

struct MSTState
{
    int cost;
    vector<vector<int>> edges;
    vector<bool> used;

    MSTState(int c, vector<vector<int>> e, vector<bool> u)
        : cost(c), edges(e), used(u) {}

    bool operator<(const MSTState &other) const
    {
        return cost > other.cost; // Min-heap based on cost
    }
};

bool compareEdges(vector<int> &a, vector<int> &b)
{
    return a[2] < b[2];
}

vector<bool> getUsedVector(int n)
{
    return vector<bool>(n, false);
}

int kthMST(int V, vector<vector<int>> &edges, int K)
{
    sort(edges.begin(), edges.end(), compareEdges);

    DSU dsu(V);
    int totalCost = 0, edgeCount = 0;
    vector<int> mstIndices;

    // Build the initial MST
    for (int i = 0; i < edges.size(); ++i)
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

    // State of MST as index set
    vector<bool> used(edges.size(), false);
    for (int idx : mstIndices)
        used[idx] = true;

    // Priority queue to store MST candidates
    priority_queue<MSTState> pq;
    map<vector<bool>, int> seen;

    pq.push(MSTState(totalCost, edges, used));
    seen[used] = totalCost;

    int count = 0;
    while (!pq.empty())
    {
        MSTState curr = pq.top();
        pq.pop();

        count++;
        if (count == K)
            return curr.cost;

        // Try replacing each edge in MST
        for (int i = 0; i < edges.size(); ++i)
        {
            if (curr.used[i])
            {
                // Temporarily remove this edge
                vector<bool> newUsed = curr.used;
                newUsed[i] = false;

                // Rebuild MST without this edge
                DSU tempDSU(V);
                int newCost = 0, newCount = 0;
                vector<bool> tempUsed(edges.size(), false);

                for (int j = 0; j < edges.size(); ++j)
                {
                    if (j == i)
                        continue;
                    int u = edges[j][0], v = edges[j][1], w = edges[j][2];
                    if (tempDSU.unite(u, v))
                    {
                        newCost += w;
                        tempUsed[j] = true;
                        if (++newCount == V - 1)
                            break;
                    }
                }

                if (newCount == V - 1)
                {
                    if (seen.find(tempUsed) == seen.end())
                    {
                        seen[tempUsed] = newCost;
                        pq.push(MSTState(newCost, edges, tempUsed));
                    }
                }
            }
        }
    }

    return -1; // Not enough MSTs
}

// Example usage
int main()
{
    vector<vector<int>> graph = {
        {0, 1, 18}, {0, 6, 2}, {0, 5, 8}, {0, 4, 10}, {1, 6, 12}, {1, 2, 4}, {2, 6, 14}, {2, 3, 26}, {3, 4, 16}, {3, 6, 30}, {4, 5, 5}, {4, 6, 3}};

    int K = 2;
    cout << "The " << K << "-th smallest MST cost is: " << kthMST(7, graph, K) << endl;

    return 0;
}