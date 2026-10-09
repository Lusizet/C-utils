#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

typedef struct Server {
    int port;
    int domain;
    int service;
    int protocol;
    int backlog;
    u_long e_interface;

    SOCKET socket;
    struct sockaddr_in address;

    void (*launch) (struct Server *server);
} Server;

Server server_constructor(int port, int domain, int service, int protocol, int backlog, u_long e_interface, void (*launch) (Server*)) {
    Server server;
    
    server.port = port;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.backlog = backlog;
    server.e_interface = e_interface;

    server.address.sin_family = (short) domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.S_un.S_addr = htonl(e_interface);

    server.socket = socket(domain, service, protocol);
    if (server.socket < 0) {
        perror("Failed to initialize socket\n");
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr*) &server.address, sizeof(server.address)) < 0) {
        perror("Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, backlog) < 0) {
        perror("Failed to start listening\n");
        exit(EXIT_FAILURE);
    }

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(domain, &server.address.sin_addr, ip_str, sizeof(ip_str))) {
        printf("Server initialized on http://%s:%d\n", ip_str, port);
    }

    server.launch = launch;
    return server;
}