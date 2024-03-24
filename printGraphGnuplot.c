#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_POINTS 100

int main() {
    FILE *GNUpipe=NULL, *data_TXT=NULL;

    data_TXT = fopen("data.txt","w");
    GNUpipe = popen("gnuplot -persist", "w");

    if(GNUpipe == NULL)
    {
        printf("Falhou o GNUPIPE\n");
    }
    
    fprintf(GNUpipe, "set term x11 persist\n");
    fprintf(GNUpipe, "set title \"Learning vs Interations\"\n");

    // Loop para plotagem em tempo real
    int x = 0;
    int y;
    while (1) {
        // Gera um valor de exemplo para y (pode ser substituído pelo valor real)
        y = rand() % 100;

        // Obtém o tempo atual
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        char timeString[9];
        strftime(timeString, sizeof(timeString), "%H:%M:%S", timeinfo);

        printf("Valores são: %d e %d", x++, y);

        // Envia os valores para o Gnuplot
        fprintf(data_TXT, "%d %d\n", x++, y);
        fflush(data_TXT);

        fprintf(GNUpipe, "plot 'data.txt' using 1:2 with lines lw 8\n");
        fflush(GNUpipe);


        // Limpa o buffer de entrada
        //while (getchar() != '\n');

        // Pausa por um curto período para simular tempo real
        usleep(500000); // 500ms

    }

    // Fechando o pipe
    fclose(data_TXT);

    return 0;
}
