#include <bits/stdc++.h>
using namespace std;

// https://youtu.be/oNI0rf2P9gE?si=40SDGFEFPtSKGegB

bool floydwarshall(vector<vector<int>> &dist)
{
    int V = dist.size();
    // intermediate vertex nite eta nilam
    for (int k = 0; k < V; k++)
    {
        // row select korar jonno
        for (int i = 0; i < V; i++)
        {
            // col select korar jonno
            for (int j = 0; j < V; j++)
            {
                // infinity thakle ar kii ba update korbo
                if (dist[i][k] != 1e8 && dist[k][j] != 1e8)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    for (int i = 0; i < V; i++)
        if (dist[i][i] < 0)
            return true;
    return false;
}

int main()
{
    int INF = 100000000;
    vector<vector<int>> dist = {
        {0, 4, INF, 5, INF},
        {INF, 0, 1, INF, 6},
        {2, INF, 0, 3, INF},
        {INF, INF, 1, 0, 2},
        {1, INF, INF, 4, 0}};

    if (floydwarshall(dist))
        cout << "neg cycle" << endl;
    else
        cout << "no neg cycle" << endl;
    for (int i = 0; i < dist.size(); i++)
    {
        for (int j = 0; j < dist.size(); j++)
        {
            cout << dist[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}