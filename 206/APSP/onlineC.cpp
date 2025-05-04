// There are N labs connected by M roads. Each lab has a capacity (number of students it can hold), 
// or it might be under maintenance (indicated by capacity = -1). The roads have a walking cost (effort to move between labs).
// You’ll answer Q queries. Each query has K students, and all students start from a given lab.

// For each student:
// They choose a lab to go to with minimum walking cost.
// Once they choose a lab, its capacity decreases by 1.
// Students can’t choose:
// a lab that is full
// a lab that is under maintenance
// Students can’t pass through a lab that is under maintenance (but can pass through full labs).
// If a student can’t find any valid lab, their cost is -1.
// 🧠 Your Goal: For each query, print the cost for each of the K students as they are assigned labs one by one.
// ⚠️ Important Rule:
// You must use All-Pairs Shortest Paths (APSP) algorithms like Floyd-Warshall — don’t use Dijkstra or Bellman-Ford.

