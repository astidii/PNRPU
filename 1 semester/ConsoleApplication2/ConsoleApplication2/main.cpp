#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include "tree.h"

using namespace std;

Tree<int> tree;
bool treeUpdated = false;

// Переменные для камеры
sf::View camera;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZoom = 1.0f;

sf::RenderWindow window(sf::VideoMode(800, 600), "Binary Tree - Camera Control");

// Функция для отрисовки дерева
void drawTree() {
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
    auto edges = tree.getEdges();
    for (const auto& edge : edges) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(edge.first.first, edge.first.second), sf::Color::Black),
            sf::Vertex(sf::Vector2f(edge.second.first, edge.second.second), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }

    // Рисуем узлы
    auto nodePositions = tree.getNodePositions();
    auto nodeValues = tree.getNodeValues();

    for (size_t i = 0; i < nodePositions.size(); i++) {
        // Рисуем круг (узел)
        sf::CircleShape nodeCircle(25);
        nodeCircle.setFillColor(sf::Color(173, 216, 230));
        nodeCircle.setOutlineThickness(2);
        nodeCircle.setOutlineColor(sf::Color::Blue);
        nodeCircle.setPosition(nodePositions[i].first - 25, nodePositions[i].second - 25);
        window.draw(nodeCircle);

        // Рисуем текст
        if (font.getInfo().family != "") {
            sf::Text text;
            text.setFont(font);
            text.setString(to_string(nodeValues[i]));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);
            text.setStyle(sf::Text::Bold);

            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin(textBounds.width / 2, textBounds.height / 2);
            text.setPosition(nodePositions[i].first, nodePositions[i].second - 5);

            window.draw(text);
        }
    }

    // Возвращаем обычный вид для интерфейса
    window.setView(window.getDefaultView());

    window.display();
}

// Функция для обработки ввода камеры
void handleCameraInput() {
    float moveSpeed = 10.0f / cameraZoom; // Скорость движения зависит от зума
    float zoomSpeed = 0.1f;

    // Движение камеры
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        cameraX -= moveSpeed;
        treeUpdated = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        cameraX += moveSpeed;
        treeUpdated = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        cameraY -= moveSpeed;
        treeUpdated = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        cameraY += moveSpeed;
        treeUpdated = true;
    }

    // Зум
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) || sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
        cameraZoom -= zoomSpeed;
        if (cameraZoom < 0.1f) cameraZoom = 0.1f;
        treeUpdated = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) || sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
        cameraZoom += zoomSpeed;
        if (cameraZoom > 3.0f) cameraZoom = 3.0f;
        treeUpdated = true;
    }
}

void showMenu() {
    cout << "\n=== БИНАРНОЕ ДЕРЕВО ===" << endl;
    cout << "1. Добавить узел" << endl;
    cout << "2. Удалить узел" << endl;
    cout << "3. Найти узел" << endl;
    cout << "4. Показать все обходы" << endl;
    cout << "5. Горизонтальная печать" << endl;
    cout << "6. Вертикальная печать" << endl;
    cout << "7. Сбалансировать дерево" << endl;
    cout << "0. Выход" << endl;
    cout << "\nВыберите действие: ";
}

void consoleMenu() {
    int choice;
    int value;

    // Тестовые данные
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);
    treeUpdated = true;

    cout << "Добавлены тестовые узлы: 50, 30, 70, 20, 40, 60, 80" << endl;
    cout << "Управление камерой в графическом окне:" << endl;
    cout << "Стрелки - движение камеры" << endl;
    cout << "+ - увеличение, - - уменьшение" << endl;
    cout << "R - сброс камеры" << endl;

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите значение: ";
            cin >> value;
            tree.insert(value);
            cout << "Узел " << value << " добавлен." << endl;
            treeUpdated = true;
            break;

        case 2:
            cout << "Введите значение: ";
            cin >> value;
            if (tree.remove(value)) {
                cout << "Узел " << value << " удален." << endl;
            }
            else {
                cout << "Узел " << value << " не найден." << endl;
            }
            treeUpdated = true;
            break;

        case 3:
            cout << "Введите значение: ";
            cin >> value;
            if (tree.search(value)) {
                cout << "Узел " << value << " найден." << endl;
            }
            else {
                cout << "Узел " << value << " не найден." << endl;
            }
            break;

        case 4:
            cout << "\n--- Обходы дерева ---" << endl;
            tree.showInOrder();
            tree.showPreOrder();
            tree.showPostOrder();
            tree.showLevelOrder();
            break;

        case 5:
            tree.printHorizontal();
            break;

        case 6:
            tree.printVertical();
            break;

        case 7:
            tree.balanceTree();
            cout << "Дерево сбалансировано!" << endl;
            treeUpdated = true;
            break;

        case 0:
            cout << "Выход..." << endl;
            window.close();
            return;

        default:
            cout << "Неверный выбор!" << endl;
        }

        if (treeUpdated) {
            treeUpdated = false;
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    system("chcp 1251 > nul");

    window.setFramerateLimit(60);

    // Инициализация камеры
    camera = window.getDefaultView();

    // Запуск консольного меню в отдельном потоке
    thread consoleThread(consoleMenu);

    cout << "SFML окно создано. Используйте консоль для управления." << endl;
    cout << "Управление камерой:" << endl;
    cout << "Стрелки - движение" << endl;
    cout << "+/- - зум" << endl;
    cout << "R - сброс камеры" << endl;

    // Главный цикл SFML
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Обработка клавиш для камеры
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                // Сброс камеры
                if (event.key.code == sf::Keyboard::R) {
                    cameraX = 0;
                    cameraY = 0;
                    cameraZoom = 1.0f;
                    treeUpdated = true;
                }
            }

            // Зум колесиком мыши
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    float zoomAmount = 0.1f;
                    if (event.mouseWheelScroll.delta > 0) {
                        cameraZoom -= zoomAmount;
                    }
                    else {
                        cameraZoom += zoomAmount;
                    }
                    // Ограничения зума
                    if (cameraZoom < 0.1f) cameraZoom = 0.1f;
                    if (cameraZoom > 3.0f) cameraZoom = 3.0f;
                    treeUpdated = true;
                }
            }
        }

        // Обработка непрерывного ввода для камеры
        handleCameraInput();

        // Перерисовываем если было изменение
        if (treeUpdated) {
            drawTree();
            treeUpdated = false;
        }
        else {
            // Или просто перерисовываем с текущим состоянием
            drawTree();
        }
    }

    consoleThread.join();
    return 0;
}