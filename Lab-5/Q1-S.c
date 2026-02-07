// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 2

int client_count = 0;
int client_sockets[10];
char messages[2][100];

pthread_mutex_t lock;

void *handle_client(void *arg)
{
    int sock = *(int *)arg;
    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    getpeername(sock, (struct sockaddr *)&addr, &len);
    inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));

    recv(sock, buffer, sizeof(buffer), 0);
    buffer[strlen(buffer)] = '\0';

    printf("Received from %s:%d → %s\n",
           client_ip, ntohs(addr.sin_port), buffer);

    pthread_mutex_lock(&lock);
    client_sockets[client_count] = sock;
    strcpy(messages[client_count], buffer);
    client_count++;

    if (client_count > MAX_CLIENTS)
    {
        for (int i = 0; i < client_count; i++)
        {
            send(client_sockets[i], "terminate session", 17, 0);
            close(client_sockets[i]);
        }
        printf("More than 2 clients connected. Terminating server.\n");
        exit(0);
    }

    if (client_count == 2)
    {
        FILE *fp = fopen("data-q1.txt", "r");
        char base[100];
        if (fp == NULL)
        {
            perror("File open error");
            exit(1);
        }
        fscanf(fp, "%s", base);
        fclose(fp);

        printf("\nFinal Output:\n%s %s %s\n",
               base, messages[0], messages[1]);

        printf("Client1 Port: %d\n", ntohs(addr.sin_port));
    }

    pthread_mutex_unlock(&lock);
    close(sock);
    free(arg);
    return NULL;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server running on port %d...\n", PORT);

    while (1)
    {
        int *pclient = malloc(sizeof(int));
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        *pclient = new_socket;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, pclient);
    }
}
