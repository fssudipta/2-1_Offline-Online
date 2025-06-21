#include <bits/stdc++.h>
using namespace std;

struct Person
{
    int height, age, id;
};

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

    pair<int, vector<pair<int, int>>> maxFlowWithMatches(int source, int sink, int m, int n)
    {
        int flow = 0;
        vector<int> parent(this->n);
        vector<pair<int, int>> matches;

        while (bfs(source, sink, parent))
        {
            int current = sink;
            int woman_node = -1, man_node = -1;

            while (current != source)
            {
                int prev = parent[current];
                if (current == sink)
                {
                    woman_node = prev;
                }
                else if (prev != source)
                {
                    man_node = prev;
                }
                current = prev;
            }

            if (man_node != -1 && woman_node != -1)
            {
                int man_idx = man_node - 1;         // man nodes are 1 to m
                int woman_idx = woman_node - m - 1; // woman nodes are m+1 to m+n
                matches.push_back({man_idx, woman_idx});
            }

            for (int v = sink; v != source; v = parent[v])
            {
                int u = parent[v];
                capacity[u][v] -= 1;
                capacity[v][u] += 1;
            }

            flow++;
        }
        return {flow, matches};
    }
};

bool canMatch(const Person &man, const Person &woman)
{
    if (abs(man.height - woman.height) > 10)
        return false;

    if (abs(man.age - woman.age) > 5)
        return false;

    return true;
}

int main()
{
    int m, n; // m men, n women
    cin >> m >> n;

    vector<Person> men(m), women(n);

    for (int i = 0; i < m; i++)
    {
        cin >> men[i].height >> men[i].age >> men[i].id;
    }

    for (int i = 0; i < n; i++)
    {
        cin >> women[i].height >> women[i].age >> women[i].id;
    }

    int total_nodes = m + n + 2;
    MaxFlow flow_network(total_nodes);

    int source = 0;
    int sink = m + n + 1;

    for (int i = 1; i <= m; i++)
    {
        flow_network.addEdge(source, i, 1);
    }

    for (int j = 1; j <= n; j++)
    {
        flow_network.addEdge(m + j, sink, 1);
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (canMatch(men[i], women[j]))
            {
                int man_node = i + 1;
                int woman_node = m + j + 1;
                flow_network.addEdge(man_node, woman_node, 1);
            }
        }
    }

    auto result = flow_network.maxFlowWithMatches(source, sink, m, n);
    int max_matches = result.first;
    vector<pair<int, int>> matches = result.second;
    cout << max_matches << endl;
    for (const auto &match : matches)
    {
        int man_idx = match.first;
        int woman_idx = match.second;
        cout << "Man " << men[man_idx].id << " - Woman " << women[woman_idx].id << endl;
    }

    return 0;
}