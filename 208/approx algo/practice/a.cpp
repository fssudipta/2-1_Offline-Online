#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

class TSPApprox
{
public:
    int n;
    vector<vector<int>> dist;
    vector<vector<int>> mst_adj;

    void readInput()
    {
        cin >> n;
        dist.assign(n, vector<int>(n));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                cin >> dist[i][j];
    }

    void buildMST()
    {
        vector<int> key(n, INT_MAX);
        vector<int> parent(n, -1);
        vector<bool> inMST(n, false);
        priority_queue<pii, vector<pii>, greater<pii>> pq;

        key[0] = 0;
        pq.push({0, 0});

        while (!pq.empty())
        {
            int u = pq.top().second;
            pq.pop();

            if (inMST[u])
                continue;
            inMST[u] = true;

            for (int v = 0; v < n; ++v)
            {
                if (u != v && !inMST[v] && dist[u][v] < key[v])
                {
                    key[v] = dist[u][v];
                    parent[v] = u;
                    pq.push({key[v], v});
                }
            }
        }

        // Build MST adjacency list
        mst_adj.assign(n, vector<int>());
        for (int i = 1; i < n; ++i)
        {
            mst_adj[parent[i]].push_back(i);
            mst_adj[i].push_back(parent[i]);
        }
    }

    vector<int> dfsPreorder()
    {
        vector<bool> visited(n, false);
        vector<int> tour;

        function<void(int)> dfs = [&](int u)
        {
            visited[u] = true;
            tour.push_back(u);
            for (int v : mst_adj[u])
                if (!visited[v])
                    dfs(v);
        };

        dfs(0);
        return tour;
    }

    pair<int, vector<int>> computeTour(const vector<int> &tour)
    {
        int totalCost = 0;
        vector<int> fullTour = tour;
        fullTour.push_back(tour[0]); // return to start

        for (int i = 0; i < (int)fullTour.size() - 1; ++i)
            totalCost += dist[fullTour[i]][fullTour[i + 1]];

        return {totalCost, fullTour};
    }

    void solve()
    {
        readInput();
        buildMST();
        vector<int> tour = dfsPreorder();
        pair<int, vector<int>> result = computeTour(tour);
        int cost = result.first;
        vector<int> fullTour = result.second;

        cout << cost << '\n';
        for (int node : fullTour)
            cout << node << " ";
        cout << '\n';
    }
};

int main()
{
    TSPApprox solver;
    solver.solve();
    return 0;
}
