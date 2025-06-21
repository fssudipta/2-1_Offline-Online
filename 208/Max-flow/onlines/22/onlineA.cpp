#include<bits/stdc++.h>
using namespace std;

class MaxFlowWithPaths
{
private:
    int n;
    vector<vector<int>> capacity;
    vector<vector<int>> adj;
    vector<vector<int>> original_adj;

public:
    MaxFlowWithPaths(int n) : n(n)
    {
        capacity.assign(n, vector<int>(n, 0));
        adj.assign(n, vector<int>());
        original_adj.assign(n, vector<int>());
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
        original_adj[u].push_back(v);
        capacity[u][v] = 1;
    }

    bool bfs(int source, int sink, vector<int> &parent)
    {
        fill(parent.begin(), parent.end(), -1);
        parent[source] = -2;
        queue<int> q;
        q.push(source);

        while (!q.empty())
        {
            int cur = q.front();
            q.pop();

            for (int next : adj[cur])
            {
                if (parent[next] == -1 && capacity[cur][next] > 0)
                {
                    parent[next] = cur;
                    if (next == sink)
                        return true;
                    q.push(next);
                }
            }
        }

        return false;
    }

    pair<int, vector<vector<int>>> maxFlowWithPaths(int source, int sink)
    {
        int flow = 0;
        vector<int> parent(n);
        vector<vector<int>> paths;

        while (bfs(source, sink, parent))
        {
            vector<int> path;
            for (int v = sink; v != source; v = parent[v])
            {
                path.push_back(v);
            }
            path.push_back(source);
            reverse(path.begin(), path.end());
            paths.push_back(path);

            for (int v = sink; v != source; v = parent[v])
            {
                int u = parent[v];
                capacity[u][v] -= 1;
                capacity[v][u] += 1;
            }

            flow++;
        }

        return {flow, paths};
    }
};

void solve(int test)
{
    int n, m;
    cin >> n >> m;

    MaxFlowWithPaths flow_network(n);

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        u--;
        v--; // 0-indexed
        flow_network.addEdge(u, v);
    }

    auto result = flow_network.maxFlowWithPaths(0, n - 1); // 0-indexed
    int max_flow = result.first;
    vector<vector<int>> paths = result.second;

    cout << "Case " << test << ": " << max_flow << endl;
    // cout << max_flow << endl;

    if (max_flow == 0)
    {
        cout << "No escape route possible! The Professor needs a new plan." << endl;
    }
    else
    {
        for (const auto &path : paths)
        {
            for (int i = 0; i < path.size(); i++)
            {
                if (i > 0)
                    cout << " -> ";
                cout << path[i] + 1;
            }
            cout << endl;
        }
    }
    cout<<endl;
}

int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        solve(testCase);
        // cout << "Case " << testCase << ": " << max_flow;
    }

    return 0;
}