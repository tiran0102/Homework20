#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8084
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE] = {0};
    char buffer[BUFFER_SIZE] = {0};

    // 1. Ստեղծում ենք Սոկետը
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 2. Միանում ենք Սերվերին (Connect)
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("Connected to Server! Start chatting (type 'exit' to quit).\n");

    // 3. Չաթի հիմնական ցիկլը
    while (1) {
        // Ա) Մուտքագրում ենք հաղորդագրությունը ստեղնաշարից
        printf("Client (You): ");
        memset(message, 0, BUFFER_SIZE);
        fgets(message, BUFFER_SIZE, stdin);

        message[strcspn(message, "\r\n")] = 0; // Հեռացնում ենք տողադարձի նշանը

        // Ուղարկում ենք Սերվերին
        send(sock, message, strlen(message), 0);

        // Ստուգում ենք՝ արդյոք մենք (Կլիենտը) գրեցինք "exit"
        if (strcmp(message, "exit") == 0) {
            printf("You closed the chat.\n");
            break;
        }

        // Բ) Ստանում ենք Սերվերի պատասխանը
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = 0; // Հեռացնում ենք տողադարձի նշանը
        printf("Server: %s\n", buffer);

        // Ստուգում ենք՝ արդյոք Սերվերը գրել է "exit"
        if (strcmp(buffer, "exit") == 0) {
            printf("Exit command received from server. Closing chat...\n");
            break;
        }
    }

    // Փակում ենք սոկետը
    close(sock);
    return 0;
}
