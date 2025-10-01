#include "calculator_glut.h"
#include <sstream>
#include <iomanip>

GlutCalculator* GlutCalculator::instance = nullptr;

GlutCalculator::GlutCalculator() {
    // Инициализация данных
    revenue = costPrice = businessExpenses = administrativeExpenses = 0;
    otherIncome = otherExpenses = incomeTax = 0;
    grossProfit = profitFromSales = balanceProfit = netProfit = 0;

    calcGrossProfit = calcProfitFromSales = calcBalanceProfit = calcNetProfit = false;
    activeTextField = -1;
}

GlutCalculator* GlutCalculator::getInstance() {
    if (!instance) {
        instance = new GlutCalculator();
    }
    return instance;
}

void GlutCalculator::initialize(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 700); // Фиксированный размер
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Financial Calculator - GLUT");

    // Устанавливаем фиксированную систему координат
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 700, 0); // (0,0) - левый верхний угол
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // Цвет фона

    // Регистрация callback'ов
    glutDisplayFunc(displayCallback);
    glutMouseFunc(mouseCallback);
    glutKeyboardFunc(keyboardCallback);
}

void GlutCalculator::run() {
    glutMainLoop();
}

// Static callback методы
void GlutCalculator::displayCallback() {
    getInstance()->render();
}

void GlutCalculator::mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        getInstance()->handleMouseClick(x, y);
    }
}

void GlutCalculator::keyboardCallback(unsigned char key, int x, int y) {
    getInstance()->handleKeyboard(key);
}

void GlutCalculator::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // ===== ЗАГОЛОВОК =====
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(500, 30, "FINANCIAL CALCULATOR");

    // ===== ПОДПИСИ ПОЛЕЙ ВВОДА =====
    drawText(50, 95, "Revenue");
    drawText(50, 145, "Cost Price");
    drawText(50, 195, "Business Expenses");

    drawText(300, 95, "Admin Expenses");
    drawText(300, 145, "Other Income");
    drawText(300, 195, "Other Expenses");

    drawText(550, 95, "Income Tax");

    // ===== ПОЛЯ ВВОДА =====
    drawField(50, 100, 200, 25, std::to_string((int)revenue), activeTextField == 0);
    drawField(50, 150, 200, 25, std::to_string((int)costPrice), activeTextField == 1);
    drawField(50, 200, 200, 25, std::to_string((int)businessExpenses), activeTextField == 2);

    drawField(300, 100, 200, 25, std::to_string((int)administrativeExpenses), activeTextField == 3);
    drawField(300, 150, 200, 25, std::to_string((int)otherIncome), activeTextField == 4);
    drawField(300, 200, 200, 25, std::to_string((int)otherExpenses), activeTextField == 5);

    drawField(550, 100, 200, 25, std::to_string((int)incomeTax), activeTextField == 6);

    // ===== ЧЕКБОКСЫ =====
    drawCheckbox(50, 250, "Gross Profit", calcGrossProfit);
    drawCheckbox(50, 300, "Profit from Sales", calcProfitFromSales);
    drawCheckbox(50, 350, "Balance Profit", calcBalanceProfit);
    drawCheckbox(50, 400, "Net Profit", calcNetProfit);

    // ===== КНОПКИ =====
    drawButton(550, 250, 150, 40, "CALCULATE");
    drawButton(550, 310, 150, 40, "CLEAR");

    // ===== РЕЗУЛЬТАТЫ =====
    drawText(800, 100, "RESULTS:");
    drawText(800, 130, "Gross Profit: " + std::to_string((int)grossProfit));
    drawText(800, 160, "Sales Profit: " + std::to_string((int)profitFromSales));
    drawText(800, 190, "Balance Profit: " + std::to_string((int)balanceProfit));
    drawText(800, 220, "Net Profit: " + std::to_string((int)netProfit));

    // ===== ФОРМУЛЫ РАСЧЕТА =====
    glColor3f(0.3f, 0.3f, 0.3f); // Темно-серый цвет для формул

    drawText(50, 500, "CALCULATION FORMULAS:");
    drawText(50, 520, "Gross Profit = Revenue - Cost Price");
    drawText(50, 540, "Profit from Sales = Revenue - Cost Price - Business Expenses - Admin Expenses");
    drawText(50, 560, "Balance Profit = Profit from Sales + Other Income - Other Expenses");
    drawText(50, 580, "Net Profit = Balance Profit - Income Tax");

    glutSwapBuffers();
}

void GlutCalculator::handleMouseClick(int x, int y) {
    // Проверка текстовых полей
    int fieldsY[] = { 100, 150, 200, 100, 150, 200, 100 };
    int fieldsX[] = { 50, 50, 50, 300, 300, 300, 550 };

    for (int i = 0; i < 7; i++) {
        if (x >= fieldsX[i] && x <= fieldsX[i] + 200 &&
            y >= fieldsY[i] && y <= fieldsY[i] + 25) {
            activeTextField = i;
            glutPostRedisplay();
            return;
        }
    }

    // Проверка чекбоксов
    int checkY[] = { 250, 300, 350, 400 };
    for (int i = 0; i < 4; i++) {
        if (x >= 50 && x <= 70 && y >= checkY[i] && y <= checkY[i] + 20) {
            switch (i) {
            case 0: calcGrossProfit = !calcGrossProfit; break;
            case 1: calcProfitFromSales = !calcProfitFromSales; break;
            case 2: calcBalanceProfit = !calcBalanceProfit; break;
            case 3: calcNetProfit = !calcNetProfit; break;
            }
            glutPostRedisplay();
            return;
        }
    }

    // Проверка кнопок
    if (x >= 550 && x <= 700 && y >= 250 && y <= 290) {
        calculate();
        glutPostRedisplay();
    }
    else if (x >= 550 && x <= 700 && y >= 310 && y <= 350) {
        clearFields();
        glutPostRedisplay();
    }
}

void GlutCalculator::handleKeyboard(unsigned char key) {
    if (activeTextField >= 0) {
        double* fields[] = { &revenue, &costPrice, &businessExpenses, &administrativeExpenses,
                           &otherIncome, &otherExpenses, &incomeTax };

        if (key >= '0' && key <= '9') {
            // Добавляем цифру
            *fields[activeTextField] = *fields[activeTextField] * 10 + (key - '0');
        }
        else if (key == 8 || key == 127) {
            // Backspace - удаляем последнюю цифру
            *fields[activeTextField] = (int)(*fields[activeTextField] / 10);
        }
        else if (key == 13 || key == 27) {
            // Enter или Escape - снимаем фокус
            activeTextField = -1;
        }

        glutPostRedisplay();
    }
}

void GlutCalculator::calculate() {
    if (calcGrossProfit) {
        grossProfit = revenue - costPrice;
    }
    if (calcProfitFromSales) {
        profitFromSales = revenue - costPrice - businessExpenses - administrativeExpenses;
    }
    if (calcBalanceProfit) {
        balanceProfit = revenue - costPrice - businessExpenses - administrativeExpenses + otherIncome - otherExpenses;
    }
    if (calcNetProfit) {
        netProfit = revenue - costPrice - businessExpenses - administrativeExpenses + otherIncome - otherExpenses - incomeTax;
    }
}

void GlutCalculator::clearFields() {
    revenue = costPrice = businessExpenses = administrativeExpenses = 0;
    otherIncome = otherExpenses = incomeTax = 0;
    grossProfit = profitFromSales = balanceProfit = netProfit = 0;
    calcGrossProfit = calcProfitFromSales = calcBalanceProfit = calcNetProfit = false;
    activeTextField = -1;
}

// ===== МЕТОДЫ ОТРИСОВКИ =====

void GlutCalculator::drawText(float x, float y, const std::string& text) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

void GlutCalculator::drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void GlutCalculator::drawField(float x, float y, float width, float height, const std::string& value, bool active) {
    // Фон поля
    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(x, y, width, height);

    // Рамка
    if (active) {
        glColor3f(0.0f, 0.0f, 1.0f);
    }
    else {
        glColor3f(0.5f, 0.5f, 0.5f);
    }
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Текст значения
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(x + 5, y + 15, value);
}

void GlutCalculator::drawCheckbox(float x, float y, const std::string& label, bool checked) {
    // Квадрат чекбокса
    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(x, y, 20, 20);

    // Рамка
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + 20, y);
    glVertex2f(x + 20, y + 20);
    glVertex2f(x, y + 20);
    glEnd();

    // Галочка
    if (checked) {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(x + 5, y + 10);
        glVertex2f(x + 10, y + 15);
        glVertex2f(x + 10, y + 15);
        glVertex2f(x + 15, y + 5);
        glEnd();
    }

    // Текст
    drawText(x + 25, y + 5, label);
}

void GlutCalculator::drawButton(float x, float y, float width, float height, const std::string& label) {
    // Фон кнопки
    glColor3f(0.8f, 0.8f, 0.8f);
    drawRect(x, y, width, height);

    // Рамка
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Текст
    drawText(x + 10, y + height / 2 + 5, label);
}

double GlutCalculator::getFieldValue(int index) {
    double fields[] = { revenue, costPrice, businessExpenses, administrativeExpenses,
                      otherIncome, otherExpenses, incomeTax };
    return fields[index];
}

bool GlutCalculator::getCheckboxState(int index) {
    bool states[] = { calcGrossProfit, calcProfitFromSales, calcBalanceProfit, calcNetProfit };
    return states[index];
}