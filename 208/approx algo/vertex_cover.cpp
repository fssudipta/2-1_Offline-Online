#include <bits/stdc++.h>
using namespace std;

class Graph
{
    int V;
    list<int> *adj;

public:
    Graph(int V)
    {
        this->V = V;
        adj = new list<int>[V];
    }
    void addEdge(int v, int w)
    {
        adj[w].push_back(v);
        adj[v].push_back(w);
    }
    void printVertexCover()
    {
        bool visited[V];
        for (int i = 0; i < V; i++)
            visited[i] = false;
        list<int>::iterator i;
        for (int u = 0; u < V; u++)
        {
            if (visited[u] == false)
            {
                for (i = adj[u].begin(); i != adj[u].end(); ++i)
                {
                    int v = *i;
                    if (visited[v] == false)
                    {
                        // Add the vertices (u, v) to the result set.
                        // We make the vertex u and v visited so that
                        // all edges from/to them would be ignored
                        visited[v] = true;
                        visited[u] = true;
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < V; i++)
            if (visited[i])
                cout << i << " ";
    }
};

int main()
{
    // Create a graph given in the above diagram
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 6);

    g.printVertexCover();

    return 0;
}