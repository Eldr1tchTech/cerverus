#include <core/logger.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr = { 0 }
    };
    
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);

    int client_fd = accept(server_fd, 0, 0);

    char buffer[256] = {0};
    recv(client_fd, buffer, 256, 0);
    LOG_DEBUG("Received: %s", buffer);
    
    if (strncmp(buffer, "GET", 3) == 0) {
        send(client_fd, "POST", strlen("POST"), 0);
    }
    
    close(client_fd);
    close(server_fd);
    return 0;
}