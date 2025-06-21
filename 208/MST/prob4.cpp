// check if an edge is a part of an mst
#include <bits/stdc++.h>
using namespace std;

class dsu
{
public:
    unordered_map<int, int> parent;
    unordered_map<int, int> rank;

    int find(int x)
    {
        if (parent.count(x) == 0)
        {
            rank[x] = 1;
            parent[x] = x;
        }
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int u, int v)
    {
        int p1 = find(u), p2 = find(v);
        if (p1 != p2)
        {
            if (rank[p1] < rank[p2])
            {
                parent[p1] = p2;
            }
            else if (rank[p1] > rank[p2])
            {
                parent[p2] = p1;
            }
            else
            {
                parent[p2] = p1;
                rank[p1]++;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
};

class Solution
{
public:
    int kruskal(bool include, vector<vector<int>> &edges, int a, int b)
    {
        dsu obj;
        int total = 0;
        if (include)
        {
            for (auto edge : edges)
            {
                int u = edge[0], v = edge[1], wt = edge[2];
                if ((u == a && v == b) || (u == b && v == a))
                {
                    bool val = obj.unite(a, b);
                    total += wt;
                    break;
                }
            }
        }
        for (auto edge : edges)
        {
            int u = edge[0], v = edge[1], wt = edge[2];
            if (obj.unite(u, v))
            {
                total += wt;
            }
        }
        return total;
    }

    bool solve(vector<vector<int>> &edges, int a, int b)
    {
        sort(edges.begin(), edges.end(), [](vector<int> a, vector<int> b)
             { return a[2] < b[2]; });
        int overall = kruskal(false, edges, a, b);
        int inc = kruskal(true, edges, a, b);
        return inc == overall;
    }
};

int main()
{
    Solution obj;
    vector<vector<int>> graph = {{0, 1, 20}, {0, 2, 5}, {0, 3, 10}, {2, 3, 10}};
    int A = 2, B = 3;
    bool val = obj.solve(graph, A, B);
    if (val)
    {
        cout << "True" << endl;
    }
    else
    {
        cout << "False" << endl;
    }
    return 0;
}