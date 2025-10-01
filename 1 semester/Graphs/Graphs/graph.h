#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <functional>
#include <cmath>

const int MAX_SIZE = 50;

struct Vertex {
    int data;
    float x, y;

    Vertex(int val) : data(val), x(0), y(0) {}
};

class Graph {
private:
    std::vector<Vertex> vertices;
    int adjMatrix[MAX_SIZE][MAX_SIZE];

public:
    Graph();
    bool isEmpty();
    bool isFull();
    int getVertText(int i);
    void getAdjMatrix(int(&matrix)[MAX_SIZE][MAX_SIZE]);
    int getAmountVerts();
    int getVertPos(const int& vertex);
    void insertVertex(const int& vertex);
    void insertEdge(const int& vertex1, const int& vertex2, int weight);
    void insertEdgeWO(const int& vertex1, const int& vertex2, int weight);
    void print();
    void eraseEdge(const int& vertex1, const int& vertex2);
    void eraseVertex(const int& vertex);
    void dijkstra(int startVertex);
    void BFS(int startVertex);
    void DFS(int startVertex);
    void dfsRec(std::vector<bool>& visited, int currVertex);

    // Новый метод для очистки графа
    void clear();

    // Методы для визуализации
    void calculatePositions();
    std::vector<Vertex>& getVertices() { return vertices; }
    int getAdjValue(int i, int j) { return adjMatrix[i][j]; }
};

#endif