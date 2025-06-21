#include<bits/stdc++.h>
using namespace std;

#define INF 1e8

bool findNegCycle(int V, vector<vector<int>> &edges, vector<int>& dist) {
    for (auto &edge : edges) {
        int u = edge[0], v = edge[1], w = edge[2];
        if (dist[u] != INF && dist[v] > dist[u] + w) {
            return true;  
        }
    }
    return false;
}

int bellmanFord(int V, vector<vector<int>>& edges, vector<int>& dist) {
    for (int i = 0; i < V - 1; i++) {
        for (auto &edge : edges) {
            int u = edge[0], v = edge[1], w = edge[2];
            if (dist[u] != INF && dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
            }
        }
    }
    if (findNegCycle(V, edges, dist)) {
        return -1; 
    }
    return 0;
}

void kingdom(int capIndex[], int block[], int ep[], vector<vector<int>>& edges, int n, int m, int k, int b, int e) {
    vector<bool> isBlocked(n + 1, false);
        for (int i = 0; i < b; i++) {
        isBlocked[block[i]] = true;
    }
    vector<vector<int>> availableEdges;
    for (auto &edge : edges) {
        int u = edge[0], v = edge[1], w = edge[2];
        if (!isBlocked[u] && !isBlocked[v]) {
            availableEdges.push_back({u, v, w});
        }
    }
    vector<int> dist(n + 1, INF);
        for (int i = 0; i < k; i++) {
        int capital = capIndex[i];
        dist[capital] = 0;
    }
    if (bellmanFord(n + 1, availableEdges, dist) == -1) {
        cout << "Abyss Detected" << endl;
        return;
    }
    for (int i = 0; i < e; i++) {
        int post = ep[i];
        if (dist[post] == INF) {
            cout << "INF" << endl;
        } else {
            cout << dist[post] << endl;
        }
    }
}

int main() {
    int n, m, k, b, e;
    cout << "Enter the number of cities (n), portals (m), capitals (k), blocked cities (b), and emergency posts (e): ";
    cin >> n >> m >> k >> b >> e;
    int capIndex[k];
    cout << "Enter k capitals: ";
    for (int i = 0; i < k; i++) {
        cin >> capIndex[i];
    }
    int block[b];
    cout << "Enter b blocked cities: ";
    for (int i = 0; i < b; i++) {
        cin >> block[i];
    }
    int ep[e];
    cout << "Enter e emergency posts: ";
    for (int i = 0; i < e; i++) {
        cin >> ep[i];
    }
    vector<vector<int>> edges(m);
    cout << "Enter the portals (u, v, w): " << endl;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    kingdom(capIndex, block, ep, edges, n, m, k, b, e);

    return 0;
}
