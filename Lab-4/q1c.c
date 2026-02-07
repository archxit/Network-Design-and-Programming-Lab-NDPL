// client.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in serv;
    char buffer[300], response[500];
    int option;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);

    connect(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("Select option:\n");
    printf("1. Registration Number\n");
    printf("2. Name of the Student\n");
    printf("3. Subject Code\n");
    printf("Enter option: ");
    scanf("%d", &option);

    if (option == 1) {
        printf("Enter Registration Number: ");
    } else if (option == 2) {
        printf("Enter Name: ");
    } else if (option == 3) {
        printf("Enter Subject Code: ");
    }

    sprintf(buffer, "%d", option);
    send(sock, buffer, strlen(buffer) + 1, 0);

    recv(sock, response, sizeof(response), 0);
    printf("\nData received from server:\n%s\n", response);

    close(sock);
    return 0;
}
