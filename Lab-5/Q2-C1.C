// Q2C.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock, n;
    struct sockaddr_in serv;
    char str1[100], str2[100], buffer[200], msg[200];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);

    connect(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("Enter first string: ");
    scanf("%s", str1);

    printf("Enter second string: ");
    scanf("%s", str2);

    sprintf(msg, "%s %s", str1, str2);
    send(sock, msg, strlen(msg) + 1, 0);

    memset(buffer, 0, sizeof(buffer));
    n = recv(sock, buffer, sizeof(buffer), 0);

    if (n > 0)
        printf("Server response: %s\n", buffer);
    else
        printf("No response from server\n");

    close(sock);
    return 0;
}
