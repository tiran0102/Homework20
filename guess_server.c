#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8083
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    int secret = 42; // Գաղտնի թիվը

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

    // 5. Խաղի հիմնական ցիկլը (Loop)
    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // Մաքրում ենք բուֆերը ամեն քայլից առաջ
        
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            // Եթե կլիենտն անջատվեց, դուրս ենք գալիս ցիկլից
            break; 
        }

        buffer[strcspn(buffer, "\r\n")] = 0; // Հեռացնում ենք տողադարձի նշանները
        int guess = atoi(buffer); // String-ը դարձնում ենք Integer թիվ

        // Ստուգում ենք կլիենտի ուղարկած թիվը
        if (guess > secret) {
            char *reply = "Lower";
            send(new_socket, reply, strlen(reply), 0);
        } else if (guess < secret) {
            char *reply = "Higher";
            send(new_socket, reply, strlen(reply), 0);
        } else {
            char *reply = "Correct";
            send(new_socket, reply, strlen(reply), 0);
            break; // Ճիշտ գուշակելու դեպքում ավարտում ենք ցիկլը
        }
    }

    // Փակում ենք կապը
    close(new_socket);
    close(server_fd);
    return 0;
}
Կոդը զգուշությամբ օգտագործեք2. Կլիենտի Կոդը (guess_client.c)Ստեղծեք երկրորդ ֆայլը՝ vim guess_client.c, տեղադրեք այս կոդը, պահպանեք և դուրս եկեք։c#include <stdio.h>
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

