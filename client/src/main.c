#include <core/logger.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr = { 0 }
    };
    
    connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr));
    send(sock_fd, "GET", strlen("GET"), 0);
    
    char buffer[256] = {0};
    recv(sock_fd, buffer, 256, 0);
    LOG_DEBUG("Received: %s", buffer);
    
    close(sock_fd);
    return 0;
}