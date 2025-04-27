// Given a directed and weighted graph of N nodes and M edges, 
// the task is to find the 1st to Kth shortest path lengths 
// from node 1 to N.

#include<bits/stdc++.h>
using namespace std;
 
void findKthShortest(int edges[][3], int n, int m, int k){
    vector<vector<pair<int, int>>> g(n+1);
    for(int i=0; i<m; i++){
        g[edges[i][0]].push_back({edges[i][1], edges[i][2]});
        vector<vector<int>> dis(n+1, vector<int>(k, 1e9));
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0,1});
        dis[1][0]=0;
        while(!pq.empty()){
            int u=pq.top().second;
            int d=pq.top().first;
            pq.pop();
            if(dis[u][k-1]<d) continue;
            vector<pair<int, int>> v=g[u];
            for(int i=0; i<v.size(); i++){
                int dest=v[i].first;
                int cost=v[i].second;
                if(d+cost<dis[dest][k-1]){
                    dis[dest][k-1]=d+cost;
                    sort(dis[dest].begin(), dis[dest].end());
                    pq.push({(d+cost),dest});
                }
                for (int i = 0; i < k; i++) {
                    cout << dis[n][i] << " ";
                }
            }
        }
    }
}

int main()
{
 
    // Given Input
    int N = 4, M = 6, K = 3;
    int edges[][3]
        = { { 1, 2, 1 }, { 1, 3, 3 }, { 2, 3, 2 },
            { 2, 4, 6 }, { 3, 2, 8 }, { 3, 4, 1 } };
 
    // Function Call
    findKthShortest(edges, N, M, K);
 
    return 0;
}