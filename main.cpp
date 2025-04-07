#include "Server.h"

int main(void)
{
    Server server(8080);
    server.run();
}