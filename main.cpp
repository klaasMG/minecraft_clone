#include "application.h"

int main() {
    Application application;
    application.init_app();
    application.run_app();
    application.cleanup_app();
    return 0;
}
