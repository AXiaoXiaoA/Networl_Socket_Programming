#include "Service.h"

#include <iostream>

int main() {
    try {
        Service service(606);
        service.Start();
    }
    catch (const std::exception& e) {
        std::cerr << "����" << e.what() << std::endl;
    }
    return 0;
}
