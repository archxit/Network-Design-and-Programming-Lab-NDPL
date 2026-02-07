#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000
#define MAX 100

void sortAsc(int a[], int n) {
    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(a[i] > a[j]) {
                int t = a[i]; a[i] = a[j]; a[j] = t;
            }
}

void sortDesc(int a[], int n) {
    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(a[i] < a[j]) {
                int t = a[i]; a[i] = a[j]; a[j] = t;
            }
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server, client;
    socklen_t len;
    int n, choice, key;
    int arr[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    listen(sockfd, 1);

    printf("Server waiting...\n");

    len = sizeof(client);
    newsockfd = accept(sockfd, (struct sockaddr*)&client, &len);

    while (1) {
        recv(newsockfd, &choice, sizeof(int), 0);

        if (choice == 5)
            break;

        recv(newsockfd, &n, sizeof(int), 0);
        recv(newsockfd, arr, n * sizeof(int), 0);

        if (choice == 1) {              // SEARCH
            recv(newsockfd, &key, sizeof(int), 0);
            int found = 0;
            for (int i = 0; i < n; i++)
                if (arr[i] == key) found = 1;

            send(newsockfd, &found, sizeof(int), 0);
        }
        else if (choice == 2) {         // SORT ASC
            sortAsc(arr, n);
            send(newsockfd, arr, n * sizeof(int), 0);
        }
        else if (choice == 3) {         // SORT DESC
            sortDesc(arr, n);
            send(newsockfd, arr, n * sizeof(int), 0);
        }
        else if (choice == 4) {         // ODD / EVEN
            int odd[MAX], even[MAX];
            int o=0, e=0;

            for(int i=0;i<n;i++) {
                if(arr[i] % 2 == 0) even[e++] = arr[i];
                else odd[o++] = arr[i];
            }

            send(newsockfd, &o, sizeof(int), 0);
            send(newsockfd, odd, o * sizeof(int), 0);
            send(newsockfd, &e, sizeof(int), 0);
            send(newsockfd, even, e * sizeof(int), 0);
        }
    }

    close(newsockfd);
    close(sockfd);
    printf("Server closed\n");
    return 0;
}

