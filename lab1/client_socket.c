#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *filename = "file1_receive.txt";
    char buffer[1024] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converts IP addresses from strings to network addresses
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Get file name from server
    valread = read(sock, buffer, sizeof(buffer));
    buffer[valread] = '\0';

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    // Receive data from server and write to file
    while ((valread = read(sock, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, valread, file);
    }

    fclose(file);

    printf("File received successfully\n");

    close(sock);

    return 0;
}