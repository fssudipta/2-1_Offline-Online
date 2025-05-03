// 🧪 Problem: Too Many DSA Labs!
// 🏫 Scenario:
// In the future, the ECE department has N DSA labs, connected by M bidrectional roads. Each road has a walking cost (effort to move between labs). Every lab charges an entry fee F to maintain all that fancy lab equipment 😅.

// All students enter the lab network at Lab 1.

// Each lab can hold only a certain number of students (it has a capacity).

// Now, K students are entering one by one (student 1 enters first, then student 2, and so on…).

// 🎯 Goal for Each Student:
// Find the best lab to go to so that the total cost is minimized.

// 💰 Total Cost =
// Walking cost from Lab 1 to the chosen lab + Entry fee F

// But: - A student can’t go to a lab if it’s already full (capacity reached). - If no lab is available, the student gets a cost of -1.

// 📥 Input Format:
// N M F
// c1 c2 c3 ... cN
// u1 v1 w1
// u2 v2 w2
// ...
// uM vM wM
// K
// Where:
// N = number of labs
// M = number of roads
// F = fixed entry fee for any lab
// ci = capacity of lab i (space-separated, N values)
// Each road is between labs ui and vi with walking cost wi
// K = number of students
// 📤 Output Format:
// Print K space-separated integers — the minimum total cost for each student.

// If a student can’t find a lab with remaining space, print -1 for that student.

// 📦 Sample Input:
// 5 4 20
// 1 2 1 1 2
// 1 2 2
// 4 5 1
// 3 4 1
// 1 3 1
// 5
// ✅ Sample Output:
// 20 21 22 22 22
// 🧠 Notes:
// All students start at Lab 1.
// The graph is connected, so every lab is reachable.
// Solve using Dijkstra’s/BF Algorithm from Lab 1 to get shortest distances.
// Always assign a student to the cheapest available lab


#include <bits/stdc++.h>
using namespace std;

// Function to compute shortest distances using Dijkstra's algorithm
vector<int> dijkstra(int N, vector<vector<pair<int, int> > > &adj) {
    const int INF = 1e9;
    vector<int> dist(N + 1, INF);
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

    dist[1] = 0; // Start from Lab 1
    pq.push({0,1});

    while (!pq.empty()) {
        pair<int, int> top = pq.top();
        pq.pop();

        int current_dist = top.first;
        int u = top.second;

        if (current_dist > dist[u]) continue;

        for (vector<pair<int, int> >::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
            int v = it->first;
            int w = it->second;

            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int main() {
    int N, M, F;
    cin >> N >> M >> F;

    // Capacities of labs
    vector<int> capacities(N + 1);
    for (int i = 1; i <= N; i++) cin >> capacities[i];

    // Adjacency list for the graph
    vector<vector<pair<int, int> > > adj(N + 1);
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w)); // Bidirectional roads
    }

    // Compute shortest distances from Lab 1
    vector<int> dist = dijkstra(N, adj);

    // Create a list of labs sorted by total cost
    vector<pair<int, int> > labs; // {total_cost, lab_index}
    for (int i = 1; i <= N; i++) {
        if (dist[i] != 1e9) { // Only consider reachable labs
            labs.push_back(make_pair(dist[i] + F, i));
        }
    }

    // Sort labs by total cost
    sort(labs.begin(), labs.end());

    // Number of students
    int K;
    cin >> K;

    // Output results
    vector<int> results(K, -1); // Default to -1
    for (int i = 0; i < K; i++) {
        for (vector<pair<int, int> >::iterator it = labs.begin(); it != labs.end(); ++it) {
            int total_cost = it->first;
            int lab_index = it->second;

            if (capacities[lab_index] > 0) {
                results[i] = total_cost;
                capacities[lab_index]--;
                break;
            }
        }
    }

    // Print the results
    for (int i = 0; i < K; i++) {
        cout << results[i] << " ";
    }
    cout << "\n";

    return 0;
}