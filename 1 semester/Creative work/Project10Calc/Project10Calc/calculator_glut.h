#pragma once
#include "freeglut.h"
#include <string>
#include <vector>

class GlutCalculator {
private:
    static GlutCalculator* instance;

    // Данные калькулятора
    double revenue, costPrice, businessExpenses, administrativeExpenses;
    double otherIncome, otherExpenses, incomeTax;
    double grossProfit, profitFromSales, balanceProfit, netProfit;

    // Флаги расчета
    bool calcGrossProfit, calcProfitFromSales, calcBalanceProfit, calcNetProfit;

    // Активное текстовое поле
    int activeTextField;

public:
    GlutCalculator();
    static GlutCalculator* getInstance();

    void initialize(int argc, char** argv);
    void run();

    // Статические методы для callback'ов GLUT
    static void displayCallback();
    static void mouseCallback(int button, int state, int x, int y);
    static void keyboardCallback(unsigned char key, int x, int y);

private:
    void render();
    void handleMouseClick(int x, int y);
    void handleKeyboard(unsigned char key);
    void calculate();
    void clearFields();

    // Методы отрисовки
    void drawText(float x, float y, const std::string& text);
    void drawRect(float x, float y, float width, float height);
    void drawField(float x, float y, float width, float height, const std::string& value, bool active);
    void drawCheckbox(float x, float y, const std::string& label, bool checked);
    void drawButton(float x, float y, float width, float height, const std::string& label);

    double getFieldValue(int index);
    bool getCheckboxState(int index);
};