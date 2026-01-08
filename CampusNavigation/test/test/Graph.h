#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <unordered_map>
#include <queue>
#include <stack>
#include <list>

class Graph {
private:
    std::unordered_map<std::string, int> vertexIndexMap;
    int numVertices;
    std::vector<std::list<std::pair<int, int>>> adjacencyList;

public:
    Graph();
    std::vector<std::string> vertices;
    void addVertex(const std::string& vertex);
    void addEdge(const std::string& from, const std::string& to, int weight);
    void printGraph() const;
    std::vector<int> dijkstra(int source) const;
    std::vector<int> dijkstraWithPath(int source, std::vector<int>& prev) const;
    std::string getVertexName(int index) const;
    void printShortestPath(const std::vector<int>& prev, int destination) const;
    int getVertexIndex(const std::string& vertex) const;
    int getShortestDistance(int source, int destination) const;
    void printAllPathsFromSource(int source) const;
    // 在Graph类的public区域添加
// 计算多节点路径的总距离（start → waypoints → end）
    int calculateMultiNodeDistance(int start, const std::vector<int>& waypoints, int end) const;
    // 获取多节点路径的完整节点序列
    std::vector<std::string> getMultiNodePath(int start, const std::vector<int>& waypoints, int end) const;
};

#endif