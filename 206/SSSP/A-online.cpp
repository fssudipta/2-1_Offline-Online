// 🚌 Online on Single Source Shortest Path
// Problem Statement
// You are a National Tuition Lord, constantly traveling between cities to tutor students. Since your time is valuable, every hour spent traveling or waiting costs you money. You want to find the most cost-effective route from one city to another.

// Here’s how the cost works:

// You earn K units of money per hour, so every hour spent traveling or waiting is a lost opportunity—we call this opportunity cost.
// Every time you wait for a connecting bus in a city (excluding your starting city), you spend 1 extra hour waiting.
// Your goal
// Given: - Travel times and costs between cities, - Your hourly earning rate K, - A starting city S and a destination city D,

// Find the path that minimizes your total cost, where:

// Total cost = Travel Cost + Opportunity Cost

// Opportunity Cost = K × (Total Time Spent)
// Total Time Spent = Sum of travel times + 1 hour for each transfer between buses

// Input Format
// K                  # hourly opportunity cost
// N                  # number of cities
// X                  # number of roads
// u1 v1 t1 c1        # X lines, each with: source, destination, travel time, travel cost
// ...
// S                  # starting city
// D                  # destination city
// ui, vi: cities connected by a road
// ti: time in hours to travel that road
// ci: travel cost
// All roads are bidirectional
// Output Format
// Print the most optimal path and its associated total time and total cost:

// S->[C1->C2->...->Cy]->D T C
// Where: - T: total time taken (including 1-hour waits between buses) - C: total cost (travel + time cost)

// If there is no path from S to D, print:

// Error
// Sample Input
// 1000
// 4
// 5
// 1 2 1 2500
// 1 3 1 3000
// 1 4 2 7000
// 2 4 1 3000
// 3 4 1 2000
// 1
// 4
// Sample Output
// 1->3->4 3 8000
// Explanation:
// - Path taken: 1 → 3 → 4
// - Time: 1 (to 3) + 1 (wait) + 1 (to 4) = 3 hours
// - Travel cost = 3000 + 2000 = 5000
// - Opportunity cost = 3 × 1000 = 3000
// - Total cost = 8000


#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;       // Destination city
    int time;     // Travel time
    int cost;     // Travel cost
};

// Function to find the most cost-effective path
void findOptimalPath(int K, int N, int X, vector<vector<Edge>> &adj, int S, int D) {
    const int INF = 1e9;

    // Priority queue: (total_cost, current_time, current_city, state)
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<>> pq;

    // dist[u][state]: Minimum total cost to reach city u in state (0 or 1)
    vector<vector<int>> dist(N + 1, vector<int>(2, INF));

    // Parent array for path reconstruction
    vector<pair<int, int>> parent(N + 1, {-1, -1});

    // Start with the source city (state 0)
    dist[S][0] = 0;
    pq.push({0, 0, S, 0}); // (total_cost, current_time, current_city, state)

    while (!pq.empty()) {
        auto top = pq.top(); // Get the top element from the priority queue
        pq.pop();

        // Extract components of the tuple manually
        int total_cost = get<0>(top);
        int current_time = get<1>(top);
        int u = get<2>(top);
        int state = get<3>(top);

        // If we have already processed this city in this state, skip
        if (total_cost > dist[u][state]) continue;

        // Explore neighbors
        for (auto &edge : adj[u]) {
            int v = edge.to;
            int travel_time = edge.time;
            int travel_cost = edge.cost;

            // Calculate new time and cost
            int new_time = current_time + travel_time;
            int new_cost = total_cost + travel_cost + K * travel_time;

            // If transferring at city v (not the destination), add waiting time
            if (v != D) {
                new_time += 1; // Add 1 hour for waiting
                new_cost += K; // Add opportunity cost for waiting
            }

            // Update distance and push to priority queue
            if (new_cost < dist[v][0]) {
                dist[v][0] = new_cost;
                parent[v] = {u, state};
                pq.push({new_cost, new_time, v, 0});
            }
        }
    }

    // Reconstruct the path if possible
    if (dist[D][0] == INF) {
        cout << "Error\n";
        return;
    }

    // Backtrack to reconstruct the path
    vector<int> path;
    int current = D;
    while (current != -1) {
        path.push_back(current);
        tie(current, ignore) = parent[current];
    }
    reverse(path.begin(), path.end());

    // Output the result
    cout << path[0];
    for (int i = 1; i < path.size(); i++) {
        cout << "->" << path[i];
    }
    cout << " " << dist[D][0] / K << " " << dist[D][0] << "\n";
}

// Driver code
int main() {
    int K, N, X;
    cin >> K >> N >> X;

    // Adjacency list representation of the graph
    vector<vector<Edge>> adj(N + 1);

    // Input edges
    for (int i = 0; i < X; i++) {
        int u, v, t, c;
        cin >> u >> v >> t >> c;
        adj[u].push_back(Edge{v, t, c});
        adj[v].push_back(Edge{u, t, c}); // Bidirectional roads
    }

    // Starting and destination cities
    int S, D;
    cin >> S >> D;

    // Find and print the optimal path
    findOptimalPath(K, N, X, adj, S, D);

    return 0;
}