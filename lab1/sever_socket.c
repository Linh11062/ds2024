#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};
    char *filename = "file1.txt";

    // Create socket(domain, type, protocol)
    // Domain: AF_INET (AF_INET6) for processes connected by IPV4 (IPV6)
    // Type: SOCK_STREAM (TCP), SOCK_DGRAM (UDP)
    // Protocol value for Internet Protocol, which is 0
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set the address family to AF_INET (IPv4)
    address.sin_family = AF_INET;
    // Set the IP address to INADDR_ANY
    address.sin_addr.s_addr = INADDR_ANY;
    // htons(): convert the port number to the network byte order
    address.sin_port = htons(PORT);

    // Attach the IP address and port to the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for client connection...\n");

    // Accept
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    // Send
    send(new_socket, filename, strlen(filename), 0);

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    // Read each piece of data in the file and send it to the client
    while ((valread = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(new_socket, buffer, valread, 0);
    }

    fclose(file);

    printf("File sent successfully\n");

    close(new_socket);
    close(server_fd);

    return 0;
}