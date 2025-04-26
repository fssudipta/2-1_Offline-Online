// Given a 2D grid of size n*n, where each cell represents the cost to traverse 
// through that cell, the task is to find the minimum cost to move from the 
// top left cell to the bottom right cell. 
// From a given cell, we can move in 4 directions: left, right, up, down.

#include<bits/stdc++.h>
using namespace std;

bool isValid(int i, int j, int n){
    return i>=0 && i<n && j>=0 && j<n;
}

int mincost(vector<vector<int>>&grid){
    int n=grid.size();
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
    vector<vector<int>> cost(n, vector<int>(n, INT_MAX));
    cost[0][0]=grid[0][0];
    vector<vector<int>> dir={{-1,0}, {1,0}, {0,-1}, {0,1}};
    pq.push({grid[0][0], 0 ,0});
    while(!pq.empty()){
        vector<int> top=pq.top();
        pq.pop();
        int c=top[0], i=top[1], j=top[2];
        for(auto d:dir){
            int x=i+d[0];
            int y=j+d[1];
            if(isValid(x,y,n)&& cost[i][j]+grid[x][y]<cost[x][y]){
                cost[x][y]=cost[i][j]+grid[x][y];
                pq.push({cost[x][y],x,y});
            }
        }
    }
    return cost[n-1][n-1];
}

int main(){
    vector<vector<int>> grid = 
    {{9,4,9,9},{6,7,6,4},{8,3,3,7},{7,4,9,10}};
    
    cout << mincost(grid) << endl;
    return 0;
}