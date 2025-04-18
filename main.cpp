#include <GameWindow.hpp>



int main() {
    try {
        Application app;
        app.run();
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}