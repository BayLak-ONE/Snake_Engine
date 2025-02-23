#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int is_valid_port(int port) {
    return port > 0 && port <= 65535;
}

int main() {
    int port;
    printf("Enter port number: ");
    if (scanf("%d", &port) != 1 || !is_valid_port(port)) {
        printf("Invalid port number. Please enter a valid port number between 1 and 65535.\n");
        return 1;
    }
    char command[100];
    snprintf(command, sizeof(command), "nohup php -S localhost:%d > /dev/null 2>&1 &", port);
    system(command);
    printf("Browser Socks IN > http://localhost:%d\n", port);
    while (1) {
    }

    return 0;
}
