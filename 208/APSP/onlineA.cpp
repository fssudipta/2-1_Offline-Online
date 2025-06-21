// There are K tuition lords, each in a city, who must meet in one city within X hours.
// Each lord has a different hourly rate, and traveling causes them to lose money (called opportunity cost).
// You are given the road network between cities and the lords' current locations and hourly rates. Your task is to:
// Find a city where all lords can reach in X hours or less.
// Among such cities, choose the one that minimizes the total opportunity cost (each lord's travel time × their hourly rate).
// Print the meeting city, total cost, time taken by the slowest lord, and individual paths taken by each lord.
// If no such city exists, print:
// No meeting
// Note: You must use All-Pairs Shortest Path (like Floyd-Warshall) to solve this — not Dijkstra or Bellman-Ford.

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

const int INF = 1e9;

int main()
{
    int N, M, K, X;
    cin >> N >> M >> K >> X;

    vector<int> lord_city(K);
    vector<int> hourly_rate(K);

    for (int i = 0; i < K; ++i)
        cin >> lord_city[i];
    for (int i = 0; i < K; ++i)
        cin >> hourly_rate[i];

    vector<vector<int>> dist(N + 1, vector<int>(N + 1, INF));
    vector<vector<int>> next(N + 1, vector<int>(N + 1, -1));

    // Initialize distances
    for (int i = 1; i <= N; ++i)
    {
        dist[i][i] = 0;
        next[i][i] = i;
    }

    // Read roads
    for (int i = 0; i < M; ++i)
    {
        int u, v, t;
        cin >> u >> v >> t;
        dist[u][v] = dist[v][u] = min(dist[u][v], t);
        next[u][v] = v;
        next[v][u] = u;
    }

    // Floyd-Warshall
    for (int k = 1; k <= N; ++k)
        for (int i = 1; i <= N; ++i)
            for (int j = 1; j <= N; ++j)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }

    // Evaluate each city as meeting city
    int best_city = -1;
    long long best_cost = LLONG_MAX;
    int best_max_time = INF;

    for (int city = 1; city <= N; ++city)
    {
        long long total_cost = 0;
        int max_time = 0;
        bool valid = true;

        for (int i = 0; i < K; ++i)
        {
            int time = dist[lord_city[i]][city];
            if (time > X)
            {
                valid = false;
                break;
            }
            total_cost += (long long)time * hourly_rate[i];
            max_time = max(max_time, time);
        }

        if (valid && (total_cost < best_cost || (total_cost == best_cost && max_time < best_max_time)))
        {
            best_cost = total_cost;
            best_max_time = max_time;
            best_city = city;
        }
    }

    if (best_city == -1)
    {
        cout << "No meeting\n";
        return 0;
    }

    cout << best_city << " " << best_cost << " " << best_max_time << "\n";

    // Reconstruct path
    auto build_path = [&](int u, int v)
    {
        vector<int> path;
        if (next[u][v] == -1)
            return path;
        while (u != v)
        {
            path.push_back(u);
            u = next[u][v];
        }
        path.push_back(v);
        return path;
    };

    for (int i = 0; i < K; ++i)
    {
        int from = lord_city[i];
        if (from == best_city)
        {
            cout << from << " 0\n";
            continue;
        }
        auto path = build_path(from, best_city);
        for (int j = 0; j < path.size(); ++j)
        {
            if (j > 0)
                cout << " -> ";
            cout << path[j];
        }
        int cost = dist[from][best_city] * hourly_rate[i];
        cout << " " << cost << "\n";
    }

    return 0;
}
