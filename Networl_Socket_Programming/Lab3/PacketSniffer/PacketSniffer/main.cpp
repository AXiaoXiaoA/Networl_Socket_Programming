#include "PacketSniffer.h"

#include <iostream>

int main() {
    try {
        PacketSniffer sniffer("127.0.0.1");
        sniffer.Start();
    }
    catch (const std::exception& e) {
        std::cerr << "´íÎó£º" << e.what() << std::endl;
    }
    return 0;
}
