#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8083
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

    // 3. Խաղի հիմնական ցիկլը (Loop)
    while (1) {
        printf("Guess the number: ");
        memset(message, 0, BUFFER_SIZE);
        fgets(message, BUFFER_SIZE, stdin);

        // Ուղարկում ենք թիվը սերվերին
        send(sock, message, strlen(message), 0);

        memset(buffer, 0, BUFFER_SIZE);
        // Ստանում ենք սերվերի պատասխանը
        read(sock, buffer, BUFFER_SIZE);
        printf("Server response: %s\n", buffer);

        // Եթե սերվերը պատասխանեց "Correct", ավարտում ենք խաղը
        if (strcmp(buffer, "Correct") == 0) {
            printf("You won!\n");
            break;
        }
    }

    // Փակում ենք սոկետը
    close(sock);
    return 0;
}
