#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
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

    // 4. Ընդունում ենք Կլիենտի միացումը (Accept)
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 5. Կարդում ենք սենսորի տվյալը
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        // Հեռացնում ենք տողադարձի նշանները
        buffer[strcspn(buffer, "\r\n")] = 0;

        // 6. Տեքստից առանձնացնում ենք թիվը (Օրինակ՝ "TEMP:45")
        // sscanf-ը կփնտրի "TEMP:" տեքստը և դրանից հետո եկող թիվը կպահի temp փոփոխականում
        int temp = 0;
        if (sscanf(buffer, "TEMP:%d", &temp) == 1) {
            
            // 7. Ստուգում ենք պայմանը
            if (temp > 50) {
                strcpy(response, "[ALERT] Dangerously high!");
            } else {
                strcpy(response, "[OK] Temperature normal");
            }
        } else {
            strcpy(response, "[ERROR] Invalid format! Use TEMP:value");
        }

        // 8. Պատասխանն ուղարկում ենք ետ՝ կլիենտին
        send(new_socket, response, strlen(response), 0);
    }

    // Փակում ենք կապը
    close(new_socket);
    close(server_fd);
    return 0;
}

