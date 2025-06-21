// min cost to provide water
//  Given an integer N, where N denotes the number of villages numbered 1 to N, an array wells[] where wells[i]
//  denotes the cost to build a water well in the i'th city, a 2D array pipes in form of [X Y C] which denotes that
//  the cost to connect village X and Y with water pipes is C. Your task is to provide water to each and every village
//  either by building a well in the village or connecting it to some other village having water. Find the minimum cost to do so.

#include <bits/stdc++.h>
using namespace std;

int parent[1001];
int sizez[1001];

void make(int i)
{
    parent[i] = i;
    sizez[i] = 1;
}

int find(int v)
{
    if (v == parent[v])
        return v;
    return parent[v] = find(parent[v]);
}

void Union(int a, int b)
{
    a = find(a);
    b = find(b);
    if (a != b)
    {
        if (sizez[a] < sizez[b])
            swap(a, b);
        parent[b] = a;
        sizez[a] += sizez[b];
    }
}

int minCostToProvideWater(int n, vector<int> &wells, vector<vector<int>> &pipes)
{
    set<vector<int>> st;
    for (auto e : pipes)
    {
        int u = e[0];
        int v = e[1];
        int wt = e[2];
        st.insert({wt, u, v});
    }
    for (int i = 0; i < n; i++)
    {
        st.insert({wells[i], 0, i + 1});
    }
    for (int i = 0; i <= n; i++)
    {
        make(i);
    }
    int answer = 0;
    while (st.size())
    {
        auto it = st.begin();
        int wt = (*it)[0];
        int u = (*it)[1];
        int v = (*it)[2];
        if (find(u) != find(v))
        {
            answer += wt;
            Union(u, v);
        }
        st.erase(st.begin());
    }
    return answer;
}

int main()
{
    int N = 3;
    vector<int> wells = {1, 2, 2};
    vector<vector<int>> pipes = {{1, 2, 1}, {2, 3, 1}};
    cout << minCostToProvideWater(N, wells, pipes);
    return 0;
}