#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <sstream>
#include "graph.h"

using namespace std;

Graph graph;
bool graphUpdated = false;

// Переменные для камеры
sf::View camera;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZoom = 1.0f;

sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Visualization - SFML");

// Функция для отрисовки графа
void drawGraph() {
    window.clear(sf::Color::White);

    // Устанавливаем камеру
    camera.setCenter(cameraX, cameraY);
    camera.setSize(800 * cameraZoom, 600 * cameraZoom);
    window.setView(camera);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Если шрифт не загружен, продолжаем без текста
    }

    // Рисуем ребра
    auto& vertices = graph.getVertices();
    int n = vertices.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int weight = graph.getAdjValue(i, j);
            if (weight != 0) {
                // Рисуем линию
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(vertices[i].x, vertices[i].y), sf::Color::Black),
                    sf::Vertex(sf::Vector2f(vertices[j].x, vertices[j].y), sf::Color::Black)
                };
                window.draw(line, 2, sf::Lines);

                // Рисуем вес ребра
                if (font.getInfo().family != "") {
                    float midX = (vertices[i].x + vertices[j].x) / 2;
                    float midY = (vertices[i].y + vertices[j].y) / 2;

                    sf::Text weightText;
                    weightText.setFont(font);
                    weightText.setString(to_string(weight));
                    weightText.setCharacterSize(14);
                    weightText.setFillColor(sf::Color::Blue);
                    weightText.setPosition(midX, midY);
                    window.draw(weightText);
                }
            }
        }
    }

    // Рисуем вершины
    for (size_t i = 0; i < vertices.size(); i++) {
        // Рисуем круг (вершину)
        sf::CircleShape vertexCircle(20);
        vertexCircle.setFillColor(sf::Color(255, 228, 196)); // Bisque
        vertexCircle.setOutlineThickness(2);
        vertexCircle.setOutlineColor(sf::Color::Red);
        vertexCircle.setPosition(vertices[i].x - 20, vertices[i].y - 20);
        window.draw(vertexCircle);

        // Рисуем номер вершины
        if (font.getInfo().family != "") {
            sf::Text vertexText;
            vertexText.setFont(font);
            vertexText.setString(to_string(vertices[i].data));
            vertexText.setCharacterSize(16);
            vertexText.setFillColor(sf::Color::Black);
            vertexText.setStyle(sf::Text::Bold);

            // Центрируем текст
            sf::FloatRect textBounds = vertexText.getLocalBounds();
            vertexText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            vertexText.setPosition(vertices[i].x, vertices[i].y - 5);

            window.draw(vertexText);
        }
    }

    // Возвращаем обычный вид для интерфейса
    window.setView(window.getDefaultView());

    window.display();
}

// Функция для обработки ввода камеры
void handleCameraInput() {
    float moveSpeed = 10.0f / cameraZoom;
    float zoomSpeed = 0.1f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        cameraX -= moveSpeed;
        graphUpdated = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        cameraX += moveSpeed;
        graphUpdated = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        cameraY -= moveSpeed;
        graphUpdated = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        cameraY += moveSpeed;
        graphUpdated = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) || sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
        cameraZoom -= zoomSpeed;
        if (cameraZoom < 0.1f) cameraZoom = 0.1f;
        graphUpdated = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) || sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
        cameraZoom += zoomSpeed;
        if (cameraZoom > 3.0f) cameraZoom = 3.0f;
        graphUpdated = true;
    }
}

void showMenu() {
    cout << "\n=== РАБОТА С ГРАФОМ ===" << endl;
    cout << "1. Обход в ширину (BFS)" << endl;
    cout << "2. Обход в глубину (DFS)" << endl;
    cout << "3. Алгоритм Дейкстры" << endl;
    cout << "4. Создать тестовый граф" << endl;
    cout << "5. Добавить вершину" << endl;
    cout << "6. Удалить вершину" << endl;
    cout << "7. Печать матрицы смежности" << endl;
    cout << "8. Добавить ребро" << endl;
    cout << "9. Удалить ребро" << endl;
    cout << "10. ОЧИСТИТЬ ГРАФ (начать с нуля)" << endl;  // НОВЫЙ ПУНКТ
    cout << "11. Создать случайный граф" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

void makeTestGraph() {
    Graph newGraph;

    // Добавляем вершины
    for (int i = 1; i <= 6; i++) {
        newGraph.insertVertex(i);
    }

    // Добавляем ребра
    newGraph.insertEdgeWO(1, 2, 5);
    newGraph.insertEdgeWO(1, 3, 3);
    newGraph.insertEdgeWO(2, 4, 2);
    newGraph.insertEdgeWO(3, 5, 4);
    newGraph.insertEdgeWO(4, 6, 1);
    newGraph.insertEdgeWO(5, 6, 6);

    graph = newGraph;
    cout << "Создан тестовый граф с 6 вершинами" << endl;
}

void makeRandomGraph() {
    Graph newGraph;
    int vertexCount = rand() % 8 + 4; // 4-11 вершин

    // Добавляем вершины
    for (int i = 1; i <= vertexCount; i++) {
        newGraph.insertVertex(i);
    }

    // Добавляем случайные ребра
    for (int i = 1; i <= vertexCount; i++) {
        for (int j = i + 1; j <= vertexCount; j++) {
            if (rand() % 3 == 0) { // 33% вероятность ребра
                int weight = rand() % 10 + 1;
                newGraph.insertEdgeWO(i, j, weight);
            }
        }
    }

    graph = newGraph;
    cout << "Создан случайный граф с " << vertexCount << " вершинами" << endl;
}

void consoleMenu() {
    int choice;
    int value, value1, value2, weight;

    cout << "Создаем тестовый граф..." << endl;
    makeTestGraph();

    cout << "Управление камерой в графическом окне:" << endl;
    cout << "Стрелки - движение камеры" << endl;
    cout << "+ - увеличение, - - уменьшение" << endl;
    cout << "R - сброс камеры" << endl;

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1: // BFS
            graph.print();
            cout << "Введите стартовую вершину: ";
            cin >> value;
            if (graph.getVertPos(value) != -1)
                graph.BFS(graph.getVertPos(value));
            else
                cout << "Вершина не найдена!" << endl;
            break;

        case 2: // DFS
            graph.print();
            cout << "Введите стартовую вершину: ";
            cin >> value;
            if (graph.getVertPos(value) != -1)
                graph.DFS(graph.getVertPos(value));
            else
                cout << "Вершина не найдена!" << endl;
            break;

        case 3: // Дейкстра
            graph.print();
            cout << "Введите стартовую вершину: ";
            cin >> value;
            if (graph.getVertPos(value) != -1)
                graph.dijkstra(graph.getVertPos(value));
            else
                cout << "Вершина не найдена!" << endl;
            break;

        case 4: // Тестовый граф
            makeTestGraph();
            graphUpdated = true;
            cameraX = 0; cameraY = 0; cameraZoom = 1.0f;
            break;

        case 5: // Добавить вершину
            graph.print();
            cout << "Введите номер вершины: ";
            cin >> value;
            if (graph.getVertPos(value) == -1) {
                graph.insertVertex(value);
                graphUpdated = true;
                cout << "Вершина " << value << " добавлена" << endl;
            }
            else {
                cout << "Вершина с таким номером уже существует!" << endl;
            }
            break;

        case 6: // Удалить вершину
            graph.print();
            cout << "Введите номер вершины: ";
            cin >> value;
            graph.eraseVertex(value);
            graphUpdated = true;
            break;

        case 7: // Печать матрицы
            graph.print();
            break;

        case 8: // Добавить ребро
            graph.print();
            cout << "Введите первую вершину: ";
            cin >> value1;
            cout << "Введите вторую вершину: ";
            cin >> value2;
            cout << "Введите вес ребра: ";
            cin >> weight;
            graph.insertEdgeWO(value1, value2, weight);
            graphUpdated = true;
            break;

        case 9: // Удалить ребро
            graph.print();
            cout << "Введите первую вершину: ";
            cin >> value1;
            cout << "Введите вторую вершину: ";
            cin >> value2;
            graph.eraseEdge(value1, value2);
            graphUpdated = true;
            break;

        case 10: // ОЧИСТИТЬ ГРАФ - НОВЫЙ ПУНКТ
            graph.clear();
            graphUpdated = true;
            cameraX = 0; cameraY = 0; cameraZoom = 1.0f;
            cout << "Граф очищен. Можете начать построение с нуля!" << endl;
            break;

        case 11: // Случайный граф
            makeRandomGraph();
            graphUpdated = true;
            cameraX = 0; cameraY = 0; cameraZoom = 1.0f;
            break;

        case 0:
            cout << "Выход..." << endl;
            window.close();
            return;

        default:
            cout << "Неверный выбор!" << endl;
        }

        if (graphUpdated) {
            graphUpdated = false;
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    system("chcp 1251 > nul");
    srand(time(nullptr));

    window.setFramerateLimit(60);
    camera = window.getDefaultView();

    thread consoleThread(consoleMenu);

    cout << "SFML окно создано. Используйте консоль для управления графом." << endl;
    cout << "Для начала с чистого листа используйте пункт 10 - ОЧИСТИТЬ ГРАФ" << endl;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::R) {
                    cameraX = 0; cameraY = 0; cameraZoom = 1.0f;
                    graphUpdated = true;
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    float zoomAmount = 0.1f;
                    if (event.mouseWheelScroll.delta > 0) {
                        cameraZoom -= zoomAmount;
                    }
                    else {
                        cameraZoom += zoomAmount;
                    }
                    if (cameraZoom < 0.1f) cameraZoom = 0.1f;
                    if (cameraZoom > 3.0f) cameraZoom = 3.0f;
                    graphUpdated = true;
                }
            }
        }

        handleCameraInput();

        if (graphUpdated) {
            drawGraph();
            graphUpdated = false;
        }
        else {
            drawGraph();
        }
    }

    consoleThread.join();
    return 0;
}