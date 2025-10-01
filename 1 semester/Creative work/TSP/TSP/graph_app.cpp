#include "graph_app.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>

GraphApp* GraphApp::instance = nullptr;

GraphApp::GraphApp() : nextNodeId(1), dragging(false), selectedNode(-1) {
    // Начальные вершины
    addNode(200, 200);
    addNode(400, 200);
    addNode(300, 400);
}

GraphApp* GraphApp::getInstance() {
    if (!instance) {
        instance = new GraphApp();
    }
    return instance;
}

void GraphApp::initialize(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Graph Application - TSP Solver");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 700, 0);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.9f, 0.95f, 1.0f, 1.0f);

    glutDisplayFunc(displayCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
    glutKeyboardFunc(keyboardCallback);
}

void GraphApp::run() {
    glutMainLoop();
}

// Static callback методы
void GraphApp::displayCallback() {
    getInstance()->render();
}

void GraphApp::mouseCallback(int button, int state, int x, int y) {
    getInstance()->handleMouseClick(button, state, x, y);
}

void GraphApp::motionCallback(int x, int y) {
    getInstance()->handleMouseMotion(x, y);
}

void GraphApp::keyboardCallback(unsigned char key, int x, int y) {
    getInstance()->handleKeyboard(key);
}

void GraphApp::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // ===== ЗАГОЛОВОК =====
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(400, 30, "GRAPH APPLICATION - TSP SOLVER");

    // ===== ОТРИСОВКА РЕБЕР =====
    for (const auto& edge : edges) {
        drawEdge(edge.from, edge.to, edge.weight);
    }

    // ===== ОТРИСОВКА ВЕРШИН =====
    for (const auto& node : nodes) {
        bool selected = (node.id == selectedNode);
        drawNode(node.x, node.y, node.id, selected);
    }

    // ===== ПАНЕЛЬ КНОПОК =====
    drawButton(50, 600, 150, 40, "PRINT MATRIX");
    drawButton(220, 600, 150, 40, "ADD NODE");
    drawButton(390, 600, 150, 40, "REMOVE NODE");
    drawButton(560, 600, 150, 40, "ADD EDGE");
    drawButton(730, 600, 150, 40, "REMOVE EDGE");
    drawButton(400, 650, 200, 40, "SOLVE TSP");

    // ===== ИНСТРУКЦИЯ =====
    glColor3f(0.3f, 0.3f, 0.3f);
    drawText(50, 500, "INSTRUCTIONS:");
    drawText(50, 520, "Click empty space: Add node");
    drawText(50, 540, "Click node + drag: Move node");
    drawText(50, 560, "Press keys 1-6: Use buttons");

    glutSwapBuffers();
}

void GraphApp::handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Проверка клика по кнопкам
        if (y >= 600 && y <= 640) {
            if (x >= 50 && x <= 200) { printMatrix(); return; }
            if (x >= 220 && x <= 370) { consoleAddNode(); return; }
            if (x >= 390 && x <= 540) { consoleRemoveNode(); return; }
            if (x >= 560 && x <= 710) { consoleAddEdge(); return; }
            if (x >= 730 && x <= 880) { consoleRemoveEdge(); return; }
        }
        if (y >= 650 && y <= 690 && x >= 400 && x <= 600) {
            solveTSP(); return;
        }

        // Проверка клика по вершине
        int clickedNode = findNodeAt(x, y);
        if (clickedNode != -1) {
            dragging = true;
            selectedNode = clickedNode;
            auto it = std::find_if(nodes.begin(), nodes.end(),
                [clickedNode](const Node& n) { return n.id == clickedNode; });
            if (it != nodes.end()) {
                dragOffsetX = x - it->x;
                dragOffsetY = y - it->y;
            }
        }
        else {
            // Добавление новой вершины
            addNode(x, y);
        }
        glutPostRedisplay();
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        dragging = false;
    }
}

void GraphApp::handleMouseMotion(int x, int y) {
    if (dragging && selectedNode != -1) {
        auto it = std::find_if(nodes.begin(), nodes.end(),
            [this](const Node& n) { return n.id == selectedNode; });
        if (it != nodes.end()) {
            it->x = x - dragOffsetX;
            it->y = y - dragOffsetY;
            glutPostRedisplay();
        }
    }
}

void GraphApp::handleKeyboard(unsigned char key) {
    switch (key) {
    case '1': printMatrix(); break;
    case '2': consoleAddNode(); break;
    case '3': consoleRemoveNode(); break;
    case '4': consoleAddEdge(); break;
    case '5': consoleRemoveEdge(); break;
    case '6': solveTSP(); break;
    }
}

void GraphApp::drawNode(float x, float y, int id, bool selected) {
    // Внешний круг
    if (selected) {
        glColor3f(1.0f, 0.5f, 0.0f); // Оранжевый для выделенной
    }
    else {
        glColor3f(0.2f, 0.4f, 0.8f); // Синий для обычной
    }

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 360; i += 30) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(x + cos(angle) * 20, y + sin(angle) * 20);
    }
    glEnd();

    // Внутренний круг
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 360; i += 30) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(x + cos(angle) * 15, y + sin(angle) * 15);
    }
    glEnd();

    // Номер вершины
    glColor3f(0.0f, 0.0f, 0.0f);
    std::string idStr = std::to_string(id);
    drawText(x - 5, y - 5, idStr);
}

void GraphApp::drawEdge(int from, int to, int weight) {
    auto fromIt = std::find_if(nodes.begin(), nodes.end(),
        [from](const Node& n) { return n.id == from; });
    auto toIt = std::find_if(nodes.begin(), nodes.end(),
        [to](const Node& n) { return n.id == to; });

    if (fromIt == nodes.end() || toIt == nodes.end()) return;

    // Линия ребра
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(fromIt->x, fromIt->y);
    glVertex2f(toIt->x, toIt->y);
    glEnd();

    // Вес ребра (посередине)
    float midX = (fromIt->x + toIt->x) / 2;
    float midY = (fromIt->y + toIt->y) / 2;
    glColor3f(0.8f, 0.0f, 0.0f);
    std::string weightStr = std::to_string(weight);
    drawText(midX - 5, midY - 5, weightStr);
}

void GraphApp::drawButton(float x, float y, float width, float height, const std::string& label) {
    // Фон кнопки
    glColor3f(0.7f, 0.8f, 0.9f);
    drawRect(x, y, width, height);

    // Рамка
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Текст
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(x + 10, y + height / 2 + 5, label);
}

void GraphApp::drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

void GraphApp::drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

int GraphApp::findNodeAt(float x, float y) {
    for (const auto& node : nodes) {
        float dx = node.x - x;
        float dy = node.y - y;
        if (dx * dx + dy * dy <= 20 * 20) {
            return node.id;
        }
    }
    return -1;
}

void GraphApp::addNode(float x, float y) {
    nodes.emplace_back(x, y, nextNodeId++);
}

void GraphApp::removeNode(int nodeId) {
    // Удаляем вершину
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
        [nodeId](const Node& n) { return n.id == nodeId; }), nodes.end());

    // Удаляем связанные ребра
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [nodeId](const Edge& e) { return e.from == nodeId || e.to == nodeId; }), edges.end());
}

void GraphApp::addEdge(int from, int to, int weight) {
    // Проверяем, существует ли уже такое ребро
    for (const auto& edge : edges) {
        if ((edge.from == from && edge.to == to) || (edge.from == to && edge.to == from)) {
            std::cout << "Edge already exists!" << std::endl;
            return;
        }
    }

    // Проверяем, существуют ли вершины
    bool fromExists = std::any_of(nodes.begin(), nodes.end(),
        [from](const Node& n) { return n.id == from; });
    bool toExists = std::any_of(nodes.begin(), nodes.end(),
        [to](const Node& n) { return n.id == to; });

    if (!fromExists || !toExists) {
        std::cout << "One or both nodes don't exist!" << std::endl;
        return;
    }

    edges.emplace_back(from, to, weight);
}

void GraphApp::removeEdge(int from, int to) {
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [from, to](const Edge& e) {
            return (e.from == from && e.to == to) || (e.from == to && e.to == from);
        }), edges.end());
}

void GraphApp::printMatrix() {
    if (nodes.empty()) {
        std::cout << "Graph is empty!" << std::endl;
        return;
    }

    std::cout << "\n=== ADJACENCY MATRIX ===" << std::endl;

    // Заголовок
    std::cout << "   ";
    for (const auto& node : nodes) {
        std::cout << node.id << " ";
    }
    std::cout << std::endl;

    // Матрица
    for (const auto& node1 : nodes) {
        std::cout << node1.id << "  ";
        for (const auto& node2 : nodes) {
            bool connected = false;
            for (const auto& edge : edges) {
                if ((edge.from == node1.id && edge.to == node2.id) ||
                    (edge.from == node2.id && edge.to == node1.id)) {
                    connected = true;
                    break;
                }
            }
            std::cout << (connected ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
    std::cout << "========================" << std::endl;
}

void GraphApp::solveTSP() {
    solveTSPBranchAndBound();
    }

void GraphApp::consoleAddNode() {
    std::cout << "Enter node coordinates (x y): ";
    float x, y;
    std::cin >> x >> y;
    addNode(x, y);
    glutPostRedisplay();
}

void GraphApp::consoleRemoveNode() {
    std::cout << "Enter node ID to remove: ";
    int id;
    std::cin >> id;
    removeNode(id);
    glutPostRedisplay();
}

void GraphApp::consoleAddEdge() {
    std::cout << "Enter edge (from to weight): ";
    int from, to, weight;
    std::cin >> from >> to >> weight;

    if (weight <= 0) {
        std::cout << "Weight must be positive!" << std::endl;
        return;
    }

    addEdge(from, to, weight);
    glutPostRedisplay();
}

void GraphApp::consoleRemoveEdge() {
    std::cout << "Enter edge to remove (from to): ";
    int from, to;
    std::cin >> from >> to;
    removeEdge(from, to);
    glutPostRedisplay();
}

int GraphApp::getEdgeWeight(int from, int to) {
    for (const auto& edge : edges) {
        if ((edge.from == from && edge.to == to) || (edge.from == to && edge.to == from)) {
            return edge.weight;
        }
    }
    return -1; // Ребра не существует
}

int GraphApp::getNodeIndex(int id) {
    for (size_t i = 0; i < nodes.size(); i++) {
        if (nodes[i].id == id) return i;
    }
    return -1;
}

void GraphApp::solveTSPBranchAndBound() {
    if (nodes.size() < 2) {
        std::cout << "Need at least 2 nodes for TSP!" << std::endl;
        return;
    }

    // Показываем доступные вершины
    std::cout << "\nAvailable nodes: ";
    for (const auto& node : nodes) {
        std::cout << node.id << " ";
    }
    std::cout << std::endl;

    // Запрашиваем начальную вершину
    std::cout << "Enter starting node ID: ";
    int startNode;
    std::cin >> startNode;

    // Проверяем существование вершины
    if (getNodeIndex(startNode) == -1) {
        std::cout << "Node " << startNode << " doesn't exist!" << std::endl;
        return;
    }

    std::cout << "\n=== TSP SOLUTION (Branch and Bound) ===" << std::endl;
    std::cout << "Starting from node: " << startNode << std::endl;

    std::vector<int> bestPath;
    std::vector<bool> visited(nodes.size(), false);
    int bestCost = std::numeric_limits<int>::max();

    // Начинаем рекурсию
    std::vector<int> currentPath = { startNode };
    visited[getNodeIndex(startNode)] = true;

    bnbRecursive(startNode, currentPath, visited, 0, bestCost, bestPath);

    // Выводим результат
    if (bestCost != std::numeric_limits<int>::max()) {
        std::cout << "Optimal path: ";
        for (size_t i = 0; i < bestPath.size(); i++) {
            std::cout << bestPath[i];
            if (i < bestPath.size() - 1) std::cout << " -> ";
        }
        std::cout << " -> " << bestPath[0] << " (return to start)" << std::endl;
        std::cout << "Total distance: " << bestCost << std::endl;

        // Подсветка оптимального пути на графе
        highlightPath(bestPath);
    }
    else {
        std::cout << "No Hamiltonian cycle found!" << std::endl;
    }
    std::cout << "=====================================" << std::endl;
}

void GraphApp::bnbRecursive(int current, std::vector<int>& path,
    std::vector<bool>& visited, int currentCost,
    int& bestCost, std::vector<int>& bestPath) {
    // Если все вершины посещены
    if (path.size() == nodes.size()) {
        // Пытаемся вернуться в начало
        int returnWeight = getEdgeWeight(current, path[0]);
        if (returnWeight != -1) {
            int totalCost = currentCost + returnWeight;
            if (totalCost < bestCost) {
                bestCost = totalCost;
                bestPath = path;
                std::cout << "Found better solution: " << bestCost << std::endl;
            }
        }
        return;
    }

    // Вычисляем нижнюю границу
    int lowerBound = calculateLowerBound(visited, current);
    if (currentCost + lowerBound >= bestCost) {
        return; // Отсекаем ветвь
    }

    // Перебираем все возможные следующие вершины
    for (size_t i = 0; i < nodes.size(); i++) {
        if (!visited[i]) {
            int nextNode = nodes[i].id;
            int edgeWeight = getEdgeWeight(current, nextNode);

            if (edgeWeight != -1) { // Если ребро существует
                visited[i] = true;
                path.push_back(nextNode);

                bnbRecursive(nextNode, path, visited, currentCost + edgeWeight,
                    bestCost, bestPath);

                // Backtrack
                path.pop_back();
                visited[i] = false;
            }
        }
    }
}

int GraphApp::calculateLowerBound(const std::vector<bool>& visited, int current) {
    int bound = 0;

    // Минимальное ребро из текущей вершины к непосещенным
    int minFromCurrent = std::numeric_limits<int>::max();
    for (size_t i = 0; i < nodes.size(); i++) {
        if (!visited[i]) {
            int weight = getEdgeWeight(current, nodes[i].id);
            if (weight != -1 && weight < minFromCurrent) {
                minFromCurrent = weight;
            }
        }
    }
    if (minFromCurrent != std::numeric_limits<int>::max()) {
        bound += minFromCurrent;
    }

    // Минимальные ребра для каждой непосещенной вершины
    for (size_t i = 0; i < nodes.size(); i++) {
        if (!visited[i]) {
            int minEdge1 = std::numeric_limits<int>::max();
            int minEdge2 = std::numeric_limits<int>::max();

            // Два минимальных ребра из этой вершины
            for (size_t j = 0; j < nodes.size(); j++) {
                if (i != j) {
                    int weight = getEdgeWeight(nodes[i].id, nodes[j].id);
                    if (weight != -1) {
                        if (weight < minEdge1) {
                            minEdge2 = minEdge1;
                            minEdge1 = weight;
                        }
                        else if (weight < minEdge2) {
                            minEdge2 = weight;
                        }
                    }
                }
            }

            if (minEdge1 != std::numeric_limits<int>::max()) {
                bound += minEdge1;
                if (minEdge2 != std::numeric_limits<int>::max()) {
                    bound += minEdge2;
                }
            }
        }
    }

    return bound / 2; // Каждое ребро учтено дважды
}

void GraphApp::highlightPath(const std::vector<int>& path) {
    std::cout << "Optimal path highlighted in console output" << std::endl;
}