#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    char buffer[SIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, 5);

    printf("Server waiting...\n");

    client_socket = accept(server_socket, NULL, NULL);

    memset(buffer, 0, SIZE);

    recv(client_socket, buffer, SIZE, 0);

    printf("Received: %s\n", buffer);


    int len = strlen(buffer);

    // հակադարձում
    for(int i = 0; i < len / 2; i++)
    {
        char temp = buffer[i];
        buffer[i] = buffer[len - i - 1];
        buffer[len - i - 1] = temp;
    }


    send(client_socket, buffer, strlen(buffer), 0);


    close(client_socket);
    close(server_socket);

    return 0;
}
// test
