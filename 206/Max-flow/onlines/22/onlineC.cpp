#include <bits/stdc++.h>
using namespace std;

struct Person
{
    int experience;
    int age;
    int criminal_record;
    int origin;
    int weapon_proficiency;
    int trust_level;
    int language_bitmask;

    Person(int exp = 0, int a = 0, int cr = 0, int o = 0, int wp = 0, int tl = 0, int lb = 0)
        : experience(exp), age(a), criminal_record(cr), origin(o),
          weapon_proficiency(wp), trust_level(tl), language_bitmask(lb) {}
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
        capacity[u][v] += cap;
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

bool canPartner(const Person &gangMember, const Person &partner)
{
    if (abs(gangMember.experience - partner.experience) > 12)
    {
        return false;
    }

    if (abs(gangMember.age - partner.age) > 5)
    {
        return false;
    }

    if (gangMember.criminal_record != partner.criminal_record)
    {
        return false;
    }

    if (gangMember.origin != partner.origin)
    {
        return false;
    }

    if (gangMember.weapon_proficiency == 0 && partner.weapon_proficiency == 0)
    {
        return false;
    }

    if (gangMember.trust_level + partner.trust_level < 10)
    {
        return false;
    }

    if ((gangMember.language_bitmask & partner.language_bitmask) == 0)
    {
        return false;
    }

    return true;
}

int solve(vector<Person> &gangMembers, vector<Person> &partners)
{
    int m = gangMembers.size();
    int n = partners.size();

    int source = 0;
    int sink = m + n + 1;
    MaxFlow flow_network(sink + 1);

    for (int i = 1; i <= m; i++)
    {
        flow_network.addEdge(source, i, 1);
    }

    for (int j = m + 1; j <= m + n; j++)
    {
        flow_network.addEdge(j, sink, 1);
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (canPartner(gangMembers[i], partners[j]))
            {
                int gangNode = i + 1;
                int partnerNode = m + 1 + j;
                flow_network.addEdge(gangNode, partnerNode, 1);
            }
        }
    }

    return flow_network.maxFlow(source, sink);
}

int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        int m, n;
        cin >> m >> n;

        vector<Person> gangMembers(m);
        vector<Person> partners(n);
        for (int i = 0; i < m; i++)
        {
            cin >> gangMembers[i].experience >> gangMembers[i].age >> gangMembers[i].criminal_record >> gangMembers[i].origin >> gangMembers[i].weapon_proficiency >> gangMembers[i].trust_level >> gangMembers[i].language_bitmask;
        }
        for (int i = 0; i < n; i++)
        {
            cin >> partners[i].experience >> partners[i].age >> partners[i].criminal_record >> partners[i].origin >> partners[i].weapon_proficiency >> partners[i].trust_level >> partners[i].language_bitmask;
        }

        int result = solve(gangMembers, partners);
        cout << "Case " << testCase << ": " << result << endl;
    }

    return 0;
}
