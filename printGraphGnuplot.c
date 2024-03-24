#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_POINTS 100

void setUpGraphPlot(FILE **GNUpipe, FILE **data_TXT);

void printLevel(FILE *GNUpipe, FILE *data_TXT, int level, int x);

int main()
{
    FILE *GNUpipe = NULL, *data_TXT = NULL;

    setUpGraphPlot(&GNUpipe, &data_TXT);

    int x = 0;
    int y;
    while (1)
    {
        y = rand() % 100;
        printLevel(GNUpipe, data_TXT, y, x++);
        usleep(500000); // 500ms
    }

    fclose(data_TXT);
    return 0;
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

void printLevel(FILE *GNUpipe, FILE *data_TXT, int level, int x)
{

    printf("Valores são: %d e %d", x++, level);

    fprintf(data_TXT, "%d %d\n", x++, level);
    fflush(data_TXT);

    fprintf(GNUpipe, "plot 'data.txt' using 1:2 with lines lw 8\n");
    fflush(GNUpipe);
}