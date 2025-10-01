#include "calculator_glut.h"

int main(int argc, char** argv) {
    GlutCalculator* calculator = GlutCalculator::getInstance();
    calculator->initialize(argc, argv);
    calculator->run();
    return 0;
}