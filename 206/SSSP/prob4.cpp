// Given a 2D array arr[] of size N x M, 
// where arr[i][j] represents the value of cell (i, j). 
// You are situated in the top-left cell, (0, 0), 
// and you hope to travel to the bottom-right cell, (n-1, m-1). 
// You can move up, down, left, or right, and you wish to find 
// a path such that the maximum absolute difference in values
// between two consecutive cells of the path is minimized.

#include<bits/stdc++.h>
using namespace std;

int minval(vector<vector<int>>&arr){
    int N=arr.size(), M=arr[0].size();
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
    //pq={diff, {row, col}}
    vector<vector<int>> d(N, vector<int>(M, 1e9));
    d[0][0]=0;
    pq.push({0, {0,0}});
    int dx[]={-1, 0, 1, 0};
    int dy[]={0, 1, 0, -1};

    while(!pq.empty()){
        int diff=pq.top().first;
        int r=pq.top().second.first;
        int c=pq.top().second.second;
        pq.pop();
        if(r==N-1 && c==M-1) return diff;
        for(int i=0; i<4; i++){
            // r-1, c
            // r, c+1
            // r-1, c
            // r, c-1
            int nx=dx[i]+r;
            int ny=dy[i]+c;
            if(nx>=0 && nx<N && ny>=0 &&ny<M){
                int nf=max(abs(arr[r][c]-arr[nx][ny]), diff);
                if(nf<d[nx][ny]){
                    d[nx][ny]=nf;
                    pq.push({nf, {nx, ny}});
                }
            }
        }
    }
}