#include <stdio.h>
#include <math.h>

// Defina as constantes do sistema
#define TAU 1.0           // Constante de tempo
#define K 2.0             // Ganho
#define FEEDBACK_GAIN 1.0 // Fator de realimentação

void setUpGraphPlot(FILE **GNUpipe, FILE **data_TXT);

void printLevel(FILE *GNUpipe, FILE *data_TXT, float level, float x);

// Função para calcular a resposta do sistema ao degrau com realimentação
double nivel_tanque(double entrada, double nivel_atual, double tempo)
{
    double nivel_desejado = K * entrada;
    double realimentacao = FEEDBACK_GAIN * nivel_atual; // Realimentação
    double nivel_final = (nivel_desejado - realimentacao) * (1 - exp(-tempo / TAU));
    return nivel_atual + (nivel_final - nivel_atual) * (1 - exp(-tempo / TAU));
}

int main()
{
    FILE *GNUpipe = NULL, *data_TXT = NULL;
    setUpGraphPlot(&GNUpipe, &data_TXT);
    double entrada, nivel_atual, novo_nivel, tempo = 0.0;

    printf("Digite o valor atual do nível do tanque: ");
    scanf("%lf", &nivel_atual);

    printf("Digite o valor da entrada desejada: ");
    scanf("%lf", &entrada);

    // Determina se o tanque vai subir ou descer
    if (nivel_atual < K * entrada)
    {
        printf("O nível atual é menor que o desejado. O tanque vai subir até o nível desejado.\n");
        novo_nivel = K * entrada;
    }
    else
    {
        printf("O nível atual é maior que o desejado. O tanque vai descer até o nível desejado.\n");
        novo_nivel = K * entrada;
    }

    printf("Tempo\tNível\n");

    // Simula o nível do tanque para diferentes instantes de tempo
    while (((nivel_atual < novo_nivel && novo_nivel >= K * entrada) || (nivel_atual > novo_nivel && novo_nivel <= K * entrada)) && tempo < 10)
    {
        printf("%.2f\t%.2f\n", tempo, nivel_atual);
        printLevel(GNUpipe, data_TXT, nivel_atual, tempo);

        nivel_atual = nivel_tanque(entrada, nivel_atual, tempo);
        tempo += 0.05;
    }

    // Imprime o nível final do tanque
    printf("%.2f\t%.2f (Alcançou o nível desejado)\n", tempo, nivel_atual);
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

void printLevel(FILE *GNUpipe, FILE *data_TXT, float level, float x)
{
    fprintf(data_TXT, "%.2f %.2f\n", x, level);
    fflush(data_TXT);

    fprintf(GNUpipe, "plot 'data.txt' using 1:2 with lines lw 8\n");
    fflush(GNUpipe);
}