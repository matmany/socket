// provides required data types
#include <sys/types.h>
// has address families and socket functions
#include <sys/socket.h>
// has the sockaddr_in structure
#include <netinet/in.h>
#include <arpa/inet.h>
// header for reading and write functions
#include <fcntl.h>
#include <stdio.h>
// header to help with strings
#include <string.h>
// has macros such as EXIT_FAILURE
#include <stdlib.h>
#include <unistd.h>
// port through which connection is to be made
#define CONNECTION_PORT 4242

int main()
{
    // structure to represent the address
    struct sockaddr_in server_address;
    // Message to be sent to the server
    // char *message = "This is a message from the client";
    // storage buffer to receive messages
    char recieve_buffer[100];
    // create an Ipv4 and TCP socket
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    // Check whether the socket is created successfully
    if (socket_descriptor < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    int status = 0;
    // initialize address structure for binding
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONNECTION_PORT);
    // set address to any address available

    //server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_addr.s_addr = inet_addr("54.175.89.1");

    server_address.sin_zero[8] = '\0';
    // connect to the server
    status = connect(socket_descriptor, (struct sockaddr *)&server_address, sizeof(server_address));
    if (status < 0)
    {
        perror("Couldn't connect with the server");
        exit(EXIT_FAILURE);
    }

    char mensagem[100]; // Definindo um array para armazenar a mensagem
    printf("Digite uma mensagem: ");
    fgets(mensagem, sizeof(mensagem), stdin);
    mensagem[strcspn(mensagem, "\n")] = '\0';

    while (strcmp(mensagem, "bye") != 0)
    {
        write(socket_descriptor, mensagem, strlen(mensagem));
        // read(socket_descriptor, recieve_buffer, 100);
        // printf("Message from server: %s\n", recieve_buffer);

        printf("Digite uma mensagem: ");
        fgets(mensagem, sizeof(mensagem), stdin);
        mensagem[strcspn(mensagem, "\n")] = '\0';
    }

    // strcpy(mensagem, "bye");
    write(socket_descriptor, mensagem, strlen(mensagem));

    close(socket_descriptor);
    return 0;
}