// Dijkstra.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <memory>

using namespace std;

// Dijkstra 算法类
class Dijkstra {
private:
    int n; // 节点数量
    vector<vector<pair<int, int>>> graph; // 邻接表表示的图

public:
    // 构造函数，初始化节点数量
    Dijkstra(int nodeCount) : n(nodeCount) {
        graph.resize(n + 1);
    }

    // 添加边到图中
    void addEdge(int a, int b, int w) {
        graph[a].push_back({ b, w });
        graph[b].push_back({ a, w });
    }

    // 执行 Dijkstra 算法，返回从起点到终点的最短路径
    vector<int> findShortestPath(int start, int end) {
        // 初始化距离数组和前驱数组
        vector<int> dist(n + 1, INT_MAX);
        vector<int> prev(n + 1, -1);
        dist[start] = 0;

        // 优先队列（最小堆）
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({ 0, start });

        // Dijkstra 算法主循环
        while (!pq.empty()) {
            int currentDist = pq.top().first;
            int currentNode = pq.top().second;
            pq.pop();

            // 如果当前距离大于已知最短距离，跳过
            if (currentDist > dist[currentNode]) {
                continue;
            }

            // 遍历邻居节点
            for (auto& neighbor : graph[currentNode]) {
                int nextNode = neighbor.first;
                int weight = neighbor.second;
                int newDist = currentDist + weight;

                // 如果找到更短的路径，更新距离和前驱
                if (newDist < dist[nextNode]) {
                    dist[nextNode] = newDist;
                    prev[nextNode] = currentNode;
                    pq.push({ newDist, nextNode });
                }
            }
        }

        // 检查终点是否可达
        if (dist[end] == INT_MAX) {
            return {}; // 返回空向量表示不可达
        }

        // 回溯路径
        vector<int> path;
        int current = end;
        while (current != -1) {
            path.push_back(current);
            current = prev[current];
        }

        // 反转路径得到正确顺序
        reverse(path.begin(), path.end());
        return path;
    }
};

// TODO: 在此处引用程序需要的其他标头。
