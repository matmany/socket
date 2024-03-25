// provides required data types
#include <sys/types.h>
// holds address family and socket functions
#include <sys/socket.h>
// has the sockaddr_in structure
#include <netinet/in.h>
// has functions for read and write operations
#include <fcntl.h>
// basic C header
#include <stdio.h>
// header to help with strings
#include <string.h>
// has macros such as EXIT_FAILURE
#include <stdlib.h>
#include <unistd.h>
// port through which connection is to be made

// database
#include <stdlib.h>
#include <sqlite3.h>

#define CONNECTION_PORT 4242

void setLevel(char storage_buffer[]);
int saveLevel(int level);

int main()
{
    // server socket
    int socket_descriptor;
    // socket created by the bind function
    int client_socket;
    // buffer to store message
    char storage_buffer[80];
    int length_of_address;
    // option value for respective option_name
    int option_value = 1;
    // server and client address structures
    struct sockaddr_in server_address;
    struct sockaddr_in connection_address;
    char *message = "This is a message from the server";
    // creating the socket with IPv4 domain and TCP protocol
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    // Check if the socket is created successfully
    if (socket_descriptor < 0)
    {
        perror("Socket creation failed");
        // EXIT_FAILURE is a macro used to indicate unsuccessful execution of the program
        exit(EXIT_FAILURE);
    }
    // set options for the socket
    int status = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    // check if options are set successfully
    if (status < 0)
    {
        perror("Couldn't set options");
        exit(EXIT_FAILURE);
    }
    //  initializing structure elements for address
    server_address.sin_family = AF_INET;
    // convert port to network byte order using htons
    server_address.sin_port = htons(CONNECTION_PORT);
    // any address available
    server_address.sin_addr.s_addr = INADDR_ANY;
    // Assigning null character to the last index of the character array
    server_address.sin_zero[8] = '\0';
    // bind the socket with the values address and port from the sockaddr_in structure
    status = bind(socket_descriptor, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
    // check if the binding was successful
    if (status < 0)
    {
        perror("Couldn't bind socket");
        exit(EXIT_FAILURE);
    }
    // listen on specified port with a maximum of 4 requests
    status = listen(socket_descriptor, 4);
    // check if the socket is listening successfully
    if (status < 0)
    {
        perror("Couldn't listen for connections");
        exit(EXIT_FAILURE);
    }
    length_of_address = sizeof(connection_address);
    // accept connection signals from the client
    client_socket = accept(socket_descriptor, (struct sockaddr *)&connection_address, &length_of_address);
    // check if the server is accepting the signals from the client
    if (client_socket < 0)
    {
        perror("Couldn't establish connection with client");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int bytes_read = read(client_socket, storage_buffer, 80);
        storage_buffer[bytes_read] = '\0';
        printf("Message from client: %s \n", storage_buffer);

        if (strcmp(storage_buffer, "bye") == 0 || bytes_read == 0)
        {
            break;
        }

        setLevel(storage_buffer);
    }

    close(socket_descriptor);
    close(client_socket);
    return 0;
}

void setLevel(char storage_buffer[])
{
    int level;

    level = atoi(storage_buffer);

    if (level != 0 || (level == 0 && storage_buffer[0] == '0'))
        // printf("VALOR INTEIRO É: %d\n", level);
        saveLevel(level);
    else
        printf("A string não contem um valor inteiro");
}

int saveLevel(int level)
{
    // Abrir o arquivo para escrita
    FILE *arquivo = fopen("valor.txt", "w");

    // Verificar se o arquivo foi aberto corretamente
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return 1;
    }

    // Escrever o valor da variável no arquivo
    fprintf(arquivo, "%d", level);

    // Fechar o arquivo
    fclose(arquivo);

    // // Abrir o arquivo para leitura
    // arquivo = fopen("valor.txt", "r");

    // // Verificar se o arquivo foi aberto corretamente
    // if (arquivo == NULL)
    // {
    //     printf("Erro ao abrir o arquivo.");
    //     return 1;
    // }

    // Variável para armazenar o valor lido do arquivo
    // int valor_lido;

    // // Ler o valor do arquivo
    // fscanf(arquivo, "%d", &valor_lido);

    // // Fechar o arquivo
    // fclose(arquivo);

    // // Exibir o valor lido
    // printf("Valor lido do arquivo: %d\n", valor_lido);

    return 0;
}