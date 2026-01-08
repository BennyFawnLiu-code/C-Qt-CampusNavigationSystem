#include "Graph.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <string>

const int INF = std::numeric_limits<int>::max();

Graph::Graph() : numVertices(0) {
    vertices = { "宿舍楼", "图书馆", "食堂", "教室", "里仁学院", "信息馆", "超市", "浴池" };
    numVertices = static_cast<int>(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertexIndexMap[vertices[i]] = static_cast<int>(i);
    }
    adjacencyList.resize(numVertices);
}

void Graph::addVertex(const std::string& vertex) {
    if (vertexIndexMap.count(vertex) == 0) {
        vertices.push_back(vertex);
        vertexIndexMap[vertex] = numVertices;
        numVertices++;
        adjacencyList.emplace_back();
    }
}

void Graph::addEdge(const std::string& from, const std::string& to, int weight) {
    int fromIndex = getVertexIndex(from);
    int toIndex = getVertexIndex(to);
    if (fromIndex != -1 && toIndex != -1) {
        adjacencyList[fromIndex].emplace_back(toIndex, weight);
        adjacencyList[toIndex].emplace_back(fromIndex, weight);
    }
}

void Graph::printGraph() const {
    std::cout << "Adjacency List:" << std::endl;
    for (size_t i = 0; i < adjacencyList.size(); ++i) {
        std::cout << vertices[i] << ": ";
        for (const auto& edge : adjacencyList[i]) {
            std::cout << "(" << vertices[edge.first] << ", " << edge.second << ") ";
        }
        std::cout << std::endl;
    }
}

std::vector<int> Graph::dijkstra(int source) const {
    std::vector<int> dist(numVertices, INF);
    std::vector<bool> visited(numVertices, false);
    dist[source] = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
        std::greater<std::pair<int, int>>> pq;
    pq.emplace(0, source);

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;

        for (const auto& edge : adjacencyList[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;
}

std::vector<int> Graph::dijkstraWithPath(int source, std::vector<int>& prev) const {
    std::vector<int> dist(numVertices, INF);
    std::vector<bool> visited(numVertices, false);
    dist[source] = 0;
    prev.assign(numVertices, -1);

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
        std::greater<std::pair<int, int>>> pq;
    pq.emplace(0, source);

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;

        for (const auto& edge : adjacencyList[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;
}

std::string Graph::getVertexName(int index) const {
    if (index >= 0 && index < numVertices) {
        return vertices[index];
    }
    return "Invalid Index";
}

void Graph::printShortestPath(const std::vector<int>& prev, int destination) const {
    std::stack<int> path;
    int at = destination;
    while (at != -1) {
        path.push(at);
        at = prev[at];
    }
    std::cout << "Shortest path: ";
    while (!path.empty()) {
        std::cout << getVertexName(path.top());
        path.pop();
        if (!path.empty()) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}

int Graph::getVertexIndex(const std::string& vertex) const {
    auto it = vertexIndexMap.find(vertex);
    if (it != vertexIndexMap.end()) {
        return it->second;
    }
    return -1;
}

int Graph::getShortestDistance(int source, int destination) const {
    std::vector<int> dist = dijkstra(source);
    return dist[destination];
}

void Graph::printAllPathsFromSource(int source) const {
    std::vector<int> prev(numVertices, -1);
    std::vector<int> dist = dijkstraWithPath(source, prev);
    std::cout << "Shortest paths from " << getVertexName(source) << " to all other vertices:" << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        if (dist[i] == INF) {
            std::cout << getVertexName(i) << ": INF" << std::endl;
        }
        else {
            std::cout << getVertexName(i) << ": " << dist[i] << " ";
            printShortestPath(prev, i);
        }
    }
}
// 计算多节点路径总距离
int Graph::calculateMultiNodeDistance(int start, const std::vector<int>& waypoints, int end) const {
    std::vector<int> pathNodes = { start };
    pathNodes.insert(pathNodes.end(), waypoints.begin(), waypoints.end());
    pathNodes.push_back(end);

    int totalDist = 0;
    for (size_t i = 0; i < pathNodes.size() - 1; ++i) {
        std::vector<int> dist = dijkstra(pathNodes[i]);
        if (dist[pathNodes[i + 1]] == INF) return -1; // 不可达
        totalDist += dist[pathNodes[i + 1]];
    }
    return totalDist;
}

// 获取多节点完整路径
std::vector<std::string> Graph::getMultiNodePath(int start, const std::vector<int>& waypoints, int end) const {
    std::vector<int> pathNodes = { start };
    pathNodes.insert(pathNodes.end(), waypoints.begin(), waypoints.end());
    pathNodes.push_back(end);

    std::vector<std::string> fullPath;
    for (size_t i = 0; i < pathNodes.size() - 1; ++i) {
        std::vector<int> prev(numVertices, -1);
        dijkstraWithPath(pathNodes[i], prev);

        std::vector<std::string> segment;
        int at = pathNodes[i + 1];
        while (at != -1) {
            segment.push_back(getVertexName(at));
            at = prev[at];
        }
        std::reverse(segment.begin(), segment.end());

        if (i == 0) {
            fullPath = segment;
        }
        else {
            fullPath.insert(fullPath.end(), segment.begin() + 1, segment.end());
        }
    }
    return fullPath;
}