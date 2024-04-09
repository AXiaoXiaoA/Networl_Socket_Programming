#include "Client.h"

int main() {
    Client client("127.0.0.1", 606);
    if (client.Connect()) {
        client.Run();
    }
    return 0;
}
