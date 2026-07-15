#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8084
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE] = {0};

    // 1. Ստեղծում ենք Սոկետը
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Կապում ենք Պորտի հետ (Bind)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Սպասում ենք միացման (Listen)
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for client connection on port %d...\n", PORT);

    // 4. Ընդունում ենք Կլիենտի միացումը (Accept)
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected! Start chatting.\n");

    // 5. Չաթի հիմնական ցիկլը
    while (1) {
        // Ա) Կարդում ենք Կլիենտի հաղորդագրությունը
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = 0; // Հեռացնում ենք տողադարձի նշանը
        printf("Client: %s\n", buffer);

        // Ստուգում ենք՝ արդյոք Կլիենտը գրել է "exit"
        if (strcmp(buffer, "exit") == 0) {
            printf("Exit command received from client. Closing chat...\n");
            break;
        }

        // Բ) Մուտքագրում ենք պատասխանը ստեղնաշարից
        printf("Server (You): ");
        memset(response, 0, BUFFER_SIZE);
        fgets(response, BUFFER_SIZE, stdin);
        
        response[strcspn(response, "\r\n")] = 0; // Հեռացնում ենք տողադարձի նշանը

        // Ուղարկում ենք պատասխանը Կլիենտին
        send(new_socket, response, strlen(response), 0);

        // Ստուգում ենք՝ արդյոք մենք (Սերվերը) գրեցինք "exit"
        if (strcmp(response, "exit") == 0) {
            printf("You closed the chat.\n");
            break;
        }
    }

    // Փակում ենք սոկետները
    close(new_socket);
    close(server_fd);
    return 0;
}
