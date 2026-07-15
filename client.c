#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024


int main()
{
    int socket_fd;
    struct sockaddr_in server_addr;

    char buffer[SIZE];


    socket_fd = socket(AF_INET, SOCK_STREAM, 0);


    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1",
              &server_addr.sin_addr);


    connect(socket_fd,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr));


    printf("Enter text: ");

    fgets(buffer, SIZE, stdin);

    buffer[strcspn(buffer, "\n")] = '\0';


    send(socket_fd, buffer, strlen(buffer), 0);



    memset(buffer, 0, SIZE);


    recv(socket_fd, buffer, SIZE, 0);


    printf("Reversed text: %s\n", buffer);


    close(socket_fd);

    return 0;
}
