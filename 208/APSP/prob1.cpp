// There are n cities and m roads between them. Your task is to process q queries
// where you have to determine the length of the shortest route between two given cities.
// Input
// The first input line has three integers n, m and q: the number of cities, roads, and queries.
// Then, there are m lines describing the roads. Each line has three integers a, b and c:
// there is a road between cities a and b whose length is c. All roads are two-way roads.
// Finally, there are q lines describing the queries. Each line has two integers a and b:
// determine the length of the shortest route between cities a and b.
// Output
// Print the length of the shortest route for each query. If there is no route, print -1 instead.

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const long long INF = 1e15;

int main()
{
    int n, m, q;
    cin >> n >> m >> q;

    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, INF));

    for (int i = 1; i <= n; ++i)
        dist[i][i] = 0;

    for (int i = 0; i < m; ++i)
    {
        int a, b;
        long long c;
        cin >> a >> b >> c;
        dist[a][b] = min(dist[a][b], c);
        dist[b][a] = min(dist[b][a], c);
    }

    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    for (int i = 0; i < q; ++i)
    {
        int a, b;
        cin >> a >> b;
        if (dist[a][b] == INF)
            cout << -1 << endl;
        else
            cout << dist[a][b] << endl;
    }

    return 0;
}
