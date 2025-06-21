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
        capacity[u][v] += cap;
    }

    bool bfs(int source, int sink, vector<int> &parent)
    {
        fill(parent.begin(), parent.end(), -1);
        parent[source] = -2;
        queue<pair<int, int>> q;
        q.push({source, INT_MAX});

        while (!q.empty())
        {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : adj[cur])
            {
                if (parent[next] == -1 && capacity[cur][next] != 0)
                {
                    parent[next] = cur;
                    int new_flow = min(flow, capacity[cur][next]);
                    if (next == sink)
                        return true;
                    q.push({next, new_flow});
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
    int n, m;
    cin >> n >> m;

    vector<int> numbers(n);
    for (int i = 0; i < n; i++)
    {
        cin >> numbers[i];
    }

    // Create flow network
    // Node 0: source
    // Nodes 1 to n: numbers A1, A2, ..., AN
    // Nodes n+1 to n+m: assignments 1, 2, ..., M
    // Node n+m+1: sink

    int source = 0;
    int sink = n + m + 1;
    MaxFlow flow_network(sink + 1);

    // Connect source to all number nodes with capacity 1
    for (int i = 1; i <= n; i++)
    {
        flow_network.addEdge(source, i, 1);
    }

    // Connect all assignment nodes to sink with capacity 1
    for (int j = n + 1; j <= n + m; j++)
    {
        flow_network.addEdge(j, sink, 1);
    }

    // Connect number nodes to assignment nodes if divisible
    for (int i = 0; i < n; i++)
    {
        int number = numbers[i];
        int number_node = i + 1;

        for (int assignment = 1; assignment <= m; assignment++)
        {
            if (number % assignment == 0)
            {
                int assignment_node = n + assignment;
                flow_network.addEdge(number_node, assignment_node, 1);
            }
        }
    }

    // Find maximum flow = maximum matching
    int result = flow_network.maxFlow(source, sink);
    cout << result << endl;

    return 0;
}

/*
Sample Input:
5 8
16
2
4
1
32

Expected Output:
4

Explanation:
1. Numbers: [16, 2, 4, 1, 32], M = 8

2. Possible assignments (number → divisors ≤ 8):
   - 16 → {1, 2, 4, 8}
   - 2 → {1, 2}
   - 4 → {1, 2, 4}
   - 1 → {1}
   - 32 → {1, 2, 4, 8}

3. Flow Network Construction:
   - Source (0) connects to number nodes (1-5) with capacity 1
   - Assignment nodes (6-13) connect to sink (14) with capacity 1
   - Number nodes connect to assignment nodes if divisible

4. Maximum Bipartite Matching:
   - One optimal assignment: 16→8, 2→2, 4→4, 1→1
   - 32 cannot get a unique assignment (all its options are taken)
   - Result: 4 numbers are divisible by their assignments

Time Complexity: O(V²E) where V = N + M + 2
Space Complexity: O(V²) for adjacency matrix representation
*/