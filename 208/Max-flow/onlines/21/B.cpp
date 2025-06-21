#include <bits/stdc++.h>
using namespace std;

class MaxFlow
{
private:
    int n;
    vector<vector<int>> capacity;
    vector<vector<int>> adj;

public:
    MaxFlow(int n) : n(n)
    {
        capacity.assign(n, vector<int>(n, 0));
        adj.assign(n, vector<int>());
    }

    void addEdge(int u, int v, int cap)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v] = cap;
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

    int maxFlow(int source, int sink)
    {
        int flow = 0;
        vector<int> parent(n);

        while (bfs(source, sink, parent))
        {
            int path_flow = INT_MAX;

            for (int v = sink; v != source; v = parent[v])
            {
                int u = parent[v];
                path_flow = min(path_flow, capacity[u][v]);
            }

            for (int v = sink; v != source; v = parent[v])
            {
                int u = parent[v];
                capacity[u][v] -= path_flow;
                capacity[v][u] += path_flow;
            }

            flow += path_flow;
        }
        return flow;
    }
};

int main()
{
    int N, M; // N workers, M tasks
    cin >> N >> M;

    vector<int> worker_strength(N);
    vector<int> task_strength(M);

    for (int i = 0; i < N; i++)
    {
        cin >> worker_strength[i];
    }

    for (int i = 0; i < M; i++)
    {
        cin >> task_strength[i];
    }

    int total_nodes = N + M + 2;
    MaxFlow flow_network(total_nodes);

    int source = 0;
    int sink = N + M + 1;

    for (int i = 1; i <= N; i++)
    {
        flow_network.addEdge(source, i, 1);
    }

    for (int j = 1; j <= M; j++)
    {
        flow_network.addEdge(N + j, sink, 1);
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (worker_strength[i] >= task_strength[j])
            {
                int worker_node = i + 1;
                int task_node = N + j + 1;
                flow_network.addEdge(worker_node, task_node, 1);
            }
        }
    }

    cout << flow_network.maxFlow(source, sink) << endl;

    return 0;
}