#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000
#define MAX 100

int main() {
    int sockfd;
    struct sockaddr_in server;
    int choice, n, key;
    int arr[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    while (1) {
        printf("\n1.Search  2.Sort Asc  3.Sort Desc  4.Odd/Even  5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sockfd, &choice, sizeof(int), 0);

        if (choice == 5)
            break;

        printf("Enter number of elements: ");
        scanf("%d", &n);

        printf("Enter elements:\n");
        for(int i=0;i<n;i++)
            scanf("%d", &arr[i]);

        send(sockfd, &n, sizeof(int), 0);
        send(sockfd, arr, n * sizeof(int), 0);

        if (choice == 1) {        // SEARCH
            printf("Enter number to search: ");
            scanf("%d", &key);
            send(sockfd, &key, sizeof(int), 0);

            int found;
            recv(sockfd, &found, sizeof(int), 0);
            printf(found ? "Found\n" : "Not Found\n");
        }
        else if (choice == 2 || choice == 3) {  // SORT
            recv(sockfd, arr, n * sizeof(int), 0);
            printf("Result:\n");
            for(int i=0;i<n;i++)
                printf("%d ", arr[i]);
            printf("\n");
        }
        else if (choice == 4) {    // ODD / EVEN
            int o, e, odd[MAX], even[MAX];

            recv(sockfd, &o, sizeof(int), 0);
            recv(sockfd, odd, o * sizeof(int), 0);
            recv(sockfd, &e, sizeof(int), 0);
            recv(sockfd, even, e * sizeof(int), 0);

            printf("Odd: ");
            for(int i=0;i<o;i++) printf("%d ", odd[i]);

            printf("\nEven: ");
            for(int i=0;i<e;i++) printf("%d ", even[i]);
            printf("\n");
        }
    }

    close(sockfd);
    printf("Client closed\n");
    return 0;
}

