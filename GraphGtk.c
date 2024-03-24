#include <gtk/gtk.h>

// Estrutura para armazenar os dados do gráfico
typedef struct {
    GtkWidget *drawing_area;
    double *data;
    int length;
    int current_index;
    int max_length;
} GraphData;

// Função de desenho do gráfico
static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GraphData *data = (GraphData *)user_data;
    
    // Configurações de desenho
    cairo_set_source_rgb(cr, 0, 0, 0); // Cor preta
    cairo_set_line_width(cr, 1.0);
    
    // Desenha o gráfico de linha
    cairo_move_to(cr, 0, data->data[0]);
    for (int i = 1; i < data->current_index; i++) {
        cairo_line_to(cr, i, data->data[i]);
    }
    cairo_stroke(cr);

    return FALSE;
}

// Função para atualizar o gráfico
static gboolean update_graph(gpointer user_data) {
    GraphData *data = (GraphData *)user_data;

    // Atualiza os dados do gráfico
    // Aqui você deve implementar a lógica para obter os dados em tempo real
    // e atualizar o array data->data com os novos valores
    
    // Redesenha o gráfico
    gtk_widget_queue_draw(data->drawing_area);

    return G_SOURCE_CONTINUE;
}

int main(int argc, char *argv[]) {
    // Inicializa o GTK
    gtk_init(&argc, &argv);

    // Cria uma janela
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Gráfico em Tempo Real");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Cria uma área de desenho
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Estrutura para armazenar os dados do gráfico
    GraphData graph_data;
    graph_data.drawing_area = drawing_area;
    graph_data.length = 800; // Comprimento do gráfico
    graph_data.current_index = 0;
    graph_data.max_length = 800;
    graph_data.data = calloc(graph_data.max_length, sizeof(double));

    // Conecta o sinal de desenho da área de desenho
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_callback), &graph_data);

    // Atualiza o gráfico a cada 100 milissegundos
    g_timeout_add(100, update_graph, &graph_data);

    // Mostra todos os widgets
    gtk_widget_show_all(window);

    // Inicia o loop principal do GTK
    gtk_main();

    // Libera a memória alocada
    free(graph_data.data);

    return 0;
}
