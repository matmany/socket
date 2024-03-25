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

typedef struct
{
    double time;
    double value;
} DataPair;


void setUpSocket(int *socket_descriptor, in_addr_t add);

void setUpGraphPlot(FILE **GNUpipe, FILE **data_TXT);
void printLevel(FILE *GNUpipe, FILE *data_TXT, int level, int x);

int main()
{

    // Setup Socket
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    int receive_buffer;
    DataPair data;
    setUpSocket(&socket_descriptor, INADDR_ANY); // inet_addr("3.86.248.23");

    // Setup File
    FILE *GNUpipe = NULL, *data_TXT = NULL;
    setUpGraphPlot(&GNUpipe, &data_TXT);
    int x = 0;

    while (1)
    {
        recv(socket_descriptor, &data, sizeof(data), 0);
        printf("Valores recebidos: %lf, %lf\n", data.time, data.value);

        //int received_number = ntohl(receive_buffer);

        //printf("Valor Recebido: %d\n", received_number);
        //printLevel(GNUpipe, data_TXT, received_number, x++);
    }

    close(socket_descriptor);
    return 0;
}

void setUpSocket(int *socket_descriptor, in_addr_t add)
{
    // structure to represent the address
    struct sockaddr_in server_address;

    // Check whether the socket is created successfully
    if (*socket_descriptor < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    int status = 0;

    // initialize address structure for binding
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONNECTION_PORT);

    // set address to any address available
    server_address.sin_addr.s_addr = add;

    server_address.sin_zero[8] = '\0';

    // connect to the server
    status = connect(*socket_descriptor, (struct sockaddr *)&server_address, sizeof(server_address));
    if (status < 0)
    {
        perror("Couldn't connect with the server");
        exit(EXIT_FAILURE);
    }
}

void printLevel(FILE *GNUpipe, FILE *data_TXT, int level, int x)
{
    printf("Valores são: %d e %d", x++, level);

    fprintf(data_TXT, "%d %d\n", x++, level);
    fflush(data_TXT);

    fprintf(GNUpipe, "plot 'data.txt' using 1:2 with lines lw 8\n");
    fflush(GNUpipe);
}

void setUpGraphPlot(FILE **GNUpipe, FILE **data_TXT)
{
    *data_TXT = fopen("data.txt", "w");
    *GNUpipe = popen("gnuplot -persist", "w");

    if (*GNUpipe == NULL)
    {
        printf("Falhou o GNUPIPE\n");
    }

    fprintf(*GNUpipe, "set term x11 persist\n");
    fprintf(*GNUpipe, "set title \"Learning vs Interations\"\n");
}