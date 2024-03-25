#include <stdio.h>
#include <math.h>

// Defina as constantes do sistema
#define TAU 1.0  // Constante de tempo
#define K 2.0    // Ganho

// Função para calcular a resposta do sistema ao degrau
double resposta_degrau(double t) {
    return K * (1 - exp(-t / TAU));
}

void setUpGraphPlot(FILE **GNUpipe, FILE **data_TXT);

void printLevel(FILE *GNUpipe, FILE *data_TXT, float level, float x);

int main() {
    double tempo;
    FILE *GNUpipe = NULL, *data_TXT = NULL;
    setUpGraphPlot(&GNUpipe, &data_TXT);

    
    printf("Tempo\tResposta\n");
    
    // Simule a resposta do sistema ao degrau para diferentes instantes de tempo
    for (tempo = 0; tempo <= 5 * TAU; tempo += 0.1) {
        float resposta = resposta_degrau(tempo);
        printf("%.2f\t%.2f\n", tempo, resposta);
        printLevel(GNUpipe, data_TXT, resposta, tempo);
    }
    
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

void printLevel(FILE *GNUpipe, FILE *data_TXT, float level, float x)
{

    //printf("Valores são: %d e %d", x++, level);

    fprintf(data_TXT, "%.2f %.2f\n", x, level);
    fflush(data_TXT);

    fprintf(GNUpipe, "plot 'data.txt' using 1:2 with lines lw 8\n");
    fflush(GNUpipe);
}