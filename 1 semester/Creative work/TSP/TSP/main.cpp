#include "graph_app.h"

int main(int argc, char** argv) {
    GraphApp* app = GraphApp::getInstance();
    app->initialize(argc, argv);
    app->run();
    return 0;
}