// provides required data types
#include <sys/types.h>
// has address families and socket functions
#include <sys/socket.h>
// has the sockaddr_in structure
#include <netinet/in.h>
// header for reading and write functions
#include <fcntl.h>
#include <stdio.h>
// header to help with strings
#include <string.h>
// has macros such as EXIT_FAILURE
#include <stdlib.h>
#include <unistd.h>
// port through which connection is to be made


#define CONNECTION_PORT 4243

int main()
{
    // structure to represent the address
    struct sockaddr_in server_address;
    // Message to be sent to the server
    //char *message = "This is a message from the client";
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
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_zero[8] = '\0';
    // connect to the server
    status = connect(socket_descriptor, (struct sockaddr *)&server_address, sizeof(server_address));
    if (status < 0)
    {
        perror("Couldn't connect with the server");
        exit(EXIT_FAILURE);
    }

    int receive_buffer;
    while (1)
    {
        //read(socket_descriptor, recieve_buffer, 100);
        recv(socket_descriptor, &receive_buffer, sizeof(receive_buffer), 0);

        int received_number = ntohl(receive_buffer);

        printf("Valor no Nivel Atual: %d\n", received_number);
    }

    close(socket_descriptor);
    return 0;
}
