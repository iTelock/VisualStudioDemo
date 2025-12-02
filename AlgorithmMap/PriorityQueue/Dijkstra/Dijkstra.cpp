// Dijkstra.cpp: 定义应用程序的入口点。
//

#include "Dijkstra.h"

using namespace std;

void runCase1()
{
    int n = 5;
    int m = 6;

    // 创建 Dijkstra 对象
    Dijkstra dijkstra(n);
    vector<vector<int>> data = {
        { 1, 2, 1 },
        { 1, 3, 2 },
        { 2, 4, 3 },
        { 3, 4, 4 },
        { 3, 5, 5 },
        { 4, 5, 1 }
    };

    // 读取并添加边
    for (int i = 0; i < m; i++) {
        dijkstra.addEdge(data[i][0], data[i][1], data[i][2]);
    }

    // 计算最短路径
    vector<int> path = dijkstra.findShortestPath(1, n);

    // 输出结果
    if (path.empty()) {
        cout << -1 << endl;
    }
    else {
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i != path.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }

    return;
}

void runCase2()
{
    int n = 8;
    int m = 12;

    // 创建 Dijkstra 对象
    Dijkstra dijkstra(n);
    vector<vector<int>> data = {
        { 1, 2, 4 },
        { 1, 3, 2 },
        { 2, 3, 1 },
        { 2, 4, 7 },
        { 3, 5, 3 },
        { 3, 6, 9 },
        { 4, 5, 2 },
        { 4, 7, 5 },
        { 5, 6, 8 },
        { 5, 7, 1 },
        { 6, 8, 2 },
        { 7, 8, 3 }
    };

    // 读取并添加边
    for (int i = 0; i < m; i++) {
        dijkstra.addEdge(data[i][0], data[i][1], data[i][2]);
    }

    // 计算最短路径
    vector<int> path = dijkstra.findShortestPath(1, n);

    // 输出结果
    if (path.empty()) {
        cout << -1 << endl;
    }
    else {
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i != path.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }

    return;
}

int main()
{
    // case 1.
    runCase1();
    // case 2.
    runCase2();

    return 0;
}