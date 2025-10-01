#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

template<typename T>
class Tree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        float x, y;

        Node(T val) : data(val), left(nullptr), right(nullptr), x(0), y(0) {}
    };

    Node* root;

    // Вспомогательные методы
    Node* insert(Node* node, T value);
    Node* remove(Node* node, T value);
    Node* findMin(Node* node);
    void clear(Node* node);
    void calculatePositions(Node* node, float x, float y, float horizontalSpacing);

    // Методы для балансировки
    Node* buildBalancedTree(std::vector<T>& values, int start, int end);
    void storeInOrder(Node* node, std::vector<T>& values);

    // Методы для печати
    void printHorizontal(Node* node, const std::string& prefix, bool isLeft);
    void printVertical(Node* node, int level = 0);

public:
    Tree() : root(nullptr) {}
    ~Tree() { clear(root); }

    // Основные операции
    void insert(T value);
    bool remove(T value);
    bool search(T value);

    // Балансировка
    void balanceTree();

    // Обходы
    void showInOrder();
    void showPreOrder();
    void showPostOrder();
    void showLevelOrder();

    // Печать
    void printHorizontal();
    void printVertical();

    // Для визуализации
    std::vector<std::pair<float, float>> getNodePositions();
    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> getEdges();
    std::vector<T> getNodeValues();

private:
    void inOrder(Node* node);
    void preOrder(Node* node);
    void postOrder(Node* node);
};

// Реализация основных методов
template<typename T>
typename Tree<T>::Node* Tree<T>::insert(Node* node, T value) {
    if (node == nullptr) {
        return new Node(value);
    }

    if (value < node->data) {
        node->left = insert(node->left, value);
    }
    else if (value > node->data) {
        node->right = insert(node->right, value);
    }

    return node;
}

template<typename T>
void Tree<T>::insert(T value) {
    root = insert(root, value);
}

template<typename T>
typename Tree<T>::Node* Tree<T>::remove(Node* node, T value) {
    if (node == nullptr) return nullptr;

    if (value < node->data) {
        node->left = remove(node->left, value);
    }
    else if (value > node->data) {
        node->right = remove(node->right, value);
    }
    else {
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        Node* temp = findMin(node->right);
        node->data = temp->data;
        node->right = remove(node->right, temp->data);
    }

    return node;
}

template<typename T>
bool Tree<T>::remove(T value) {
    if (!search(value)) return false;
    root = remove(root, value);
    return true;
}

template<typename T>
typename Tree<T>::Node* Tree<T>::findMin(Node* node) {
    while (node && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template<typename T>
bool Tree<T>::search(T value) {
    Node* current = root;
    while (current != nullptr) {
        if (value == current->data) return true;
        else if (value < current->data) current = current->left;
        else current = current->right;
    }
    return false;
}

template<typename T>
void Tree<T>::clear(Node* node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

// Методы балансировки
template<typename T>
void Tree<T>::storeInOrder(Node* node, std::vector<T>& values) {
    if (node == nullptr) return;

    storeInOrder(node->left, values);
    values.push_back(node->data);
    storeInOrder(node->right, values);
}

template<typename T>
typename Tree<T>::Node* Tree<T>::buildBalancedTree(std::vector<T>& values, int start, int end) {
    if (start > end) return nullptr;

    int mid = (start + end) / 2;
    Node* node = new Node(values[mid]);

    node->left = buildBalancedTree(values, start, mid - 1);
    node->right = buildBalancedTree(values, mid + 1, end);

    return node;
}

template<typename T>
void Tree<T>::balanceTree() {
    std::vector<T> values;
    storeInOrder(root, values); // Получаем отсортированные значения

    // Очищаем старое дерево
    clear(root);

    // Строим сбалансированное дерево
    root = buildBalancedTree(values, 0, values.size() - 1);
}

// Методы печати
template<typename T>
void Tree<T>::printHorizontal() {
    std::cout << "\n=== ГОРИЗОНТАЛЬНАЯ ПЕЧАТЬ ===" << std::endl;
    printHorizontal(root, "", true);
}

template<typename T>
void Tree<T>::printVertical() {
    std::cout << "\n=== ВЕРТИКАЛЬНАЯ ПЕЧАТЬ ===" << std::endl;
    printVertical(root);
    std::cout << std::endl;
}

template<typename T>
void Tree<T>::printHorizontal(Node* node, const std::string& prefix, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "|--" : "'--");
        std::cout << node->data << std::endl;

        printHorizontal(node->left, prefix + (isLeft ? "|   " : "    "), true);
        printHorizontal(node->right, prefix + (isLeft ? "|   " : "    "), false);
    }
}

template<typename T>
void Tree<T>::printVertical(Node* node, int level) {
    if (node != nullptr) {
        printVertical(node->right, level + 1);

        for (int i = 0; i < level; i++) {
            std::cout << "    ";
        }
        std::cout << node->data << std::endl;

        printVertical(node->left, level + 1);
    }
}

// Методы обхода
template<typename T>
void Tree<T>::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        std::cout << node->data << " ";
        inOrder(node->right);
    }
}

template<typename T>
void Tree<T>::preOrder(Node* node) {
    if (node != nullptr) {
        std::cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }
}

template<typename T>
void Tree<T>::postOrder(Node* node) {
    if (node != nullptr) {
        postOrder(node->left);
        postOrder(node->right);
        std::cout << node->data << " ";
    }
}

template<typename T>
void Tree<T>::showInOrder() {
    std::cout << "Симметричный обход: ";
    inOrder(root);
    std::cout << std::endl;
}

template<typename T>
void Tree<T>::showPreOrder() {
    std::cout << "Прямой обход: ";
    preOrder(root);
    std::cout << std::endl;
}

template<typename T>
void Tree<T>::showPostOrder() {
    std::cout << "Обратный обход: ";
    postOrder(root);
    std::cout << std::endl;
}

template<typename T>
void Tree<T>::showLevelOrder() {
    std::cout << "Обход в ширину: ";
    if (root == nullptr) {
        std::cout << "Дерево пустое" << std::endl;
        return;
    }

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        std::cout << current->data << " ";

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    std::cout << std::endl;
}

// Методы для визуализации
template<typename T>
void Tree<T>::calculatePositions(Node* node, float x, float y, float horizontalSpacing) {
    if (node == nullptr) return;

    node->x = x;
    node->y = y;

    calculatePositions(node->left, x - horizontalSpacing, y + 100, horizontalSpacing / 2);
    calculatePositions(node->right, x + horizontalSpacing, y + 100, horizontalSpacing / 2);
}

template<typename T>
std::vector<std::pair<float, float>> Tree<T>::getNodePositions() {
    std::vector<std::pair<float, float>> positions;
    calculatePositions(root, 400, 100, 200);

    std::queue<Node*> q;
    if (root) q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        positions.push_back({ current->x, current->y });

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return positions;
}

template<typename T>
std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> Tree<T>::getEdges() {
    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> edges;
    calculatePositions(root, 400, 100, 200);

    std::queue<Node*> q;
    if (root) q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->left) {
            edges.push_back({ {current->x, current->y}, {current->left->x, current->left->y} });
            q.push(current->left);
        }
        if (current->right) {
            edges.push_back({ {current->x, current->y}, {current->right->x, current->right->y} });
            q.push(current->right);
        }
    }

    return edges;
}

template<typename T>
std::vector<T> Tree<T>::getNodeValues() {
    std::vector<T> values;
    calculatePositions(root, 400, 100, 200);

    std::queue<Node*> q;
    if (root) q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        values.push_back(current->data);

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return values;
}

#endif