#pragma once
#include "freeglut.h"
#include <vector>
#include <string>
#include <algorithm>

class GraphApp {
private:
    static GraphApp* instance;

    struct Node {
        float x, y;
        int id;
        Node(float x, float y, int id) : x(x), y(y), id(id) {}
    };

    struct Edge {
        int from, to, weight;
        Edge(int from, int to, int weight = 1) : from(from), to(to), weight(weight) {}
    };

    std::vector<Node> nodes;
    std::vector<Edge> edges;
    int nextNodeId;
    bool dragging;
    int selectedNode;
    float dragOffsetX, dragOffsetY;

public:
    GraphApp();
    static GraphApp* getInstance();

    void initialize(int argc, char** argv);
    void run();

    static void displayCallback();
    static void mouseCallback(int button, int state, int x, int y);
    static void motionCallback(int x, int y);
    static void keyboardCallback(unsigned char key, int x, int y);

private:
    void render();
    void handleMouseClick(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);
    void handleKeyboard(unsigned char key);

    void drawNode(float x, float y, int id, bool selected = false);
    void drawEdge(int from, int to, int weight);
    void drawButton(float x, float y, float width, float height, const std::string& label);
    void drawText(float x, float y, const std::string& text);
    void drawRect(float x, float y, float width, float height);

    int findNodeAt(float x, float y);
    void addNode(float x, float y);
    void removeNode(int nodeId);
    void addEdge(int from, int to, int weigth);
    void removeEdge(int from, int to);
    void printMatrix();
    void solveTSP();

    void consoleAddNode();
    void consoleRemoveNode();
    void consoleAddEdge();
    void consoleRemoveEdge();

    void solveTSPBranchAndBound();
    void bnbRecursive(int current, std::vector<int>& path, std::vector<bool>& visited,
        int currentCost, int& bestCost, std::vector<int>& bestPath);
    int calculateLowerBound(const std::vector<bool>& visited, int current);
    int getEdgeWeight(int from, int to);
    int getNodeIndex(int id);
    void highlightPath(const std::vector<int>& path);
};