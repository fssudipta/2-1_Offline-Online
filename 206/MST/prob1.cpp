// minimum cost to connect all the cities
//  There are n cities and there are roads in between some of the cities.
//  Somehow all the roads are damaged simultaneously. We have to repair the roads to connect the cities again.
//  There is a fixed cost to repair a particular road.
//  Input is in the form of edges {u, v, w} where, u and v are city indices.
//  w is the cost to rebuild the road between u and v. Print out the minimum cost to connect all the cities by repairing roads.
#include <bits/stdc++.h>
using namespace std;

int findMST(int n, vector<vector<int>> &edges)
{
    vector<vector<pair<int, int>>> adj(n + 1);

    for (auto &edge : edges)
    {
        int u = edge[0], v = edge[1], w = edge[2];
        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    vector<bool> inMST(n + 1, false);
    pq.push({0, 1});
    int mstCost = 0;

    while (!pq.empty())
    {
        auto tmp = pq.top();
        int w = tmp.first;
        int u = tmp.second;
        pq.pop();

        if (inMST[u])
            continue;
        inMST[u] = true;
        mstCost += w;

        for (auto &it : adj[u])
        {
            int weight = it.first;
            int v = it.second;
            if (!inMST[v])
            {
                pq.push({weight, v});
            }
        }
    }

    return mstCost;
}

int main()
{
    vector<vector<int>> city1 = {
        {1, 2, 1}, {1, 3, 2}, {1, 4, 3}, {1, 5, 4}, {2, 3, 5}, {2, 5, 7}, {3, 4, 6}};
    cout << findMST(5, city1) << endl;

    vector<vector<int>> city2 = {
        {1, 2, 1}, {1, 3, 1}, {1, 4, 100}, {2, 3, 1}, {4, 5, 2}, {4, 6, 2}, {5, 6, 2}};
    cout << findMST(6, city2) << endl;

    return 0;
}