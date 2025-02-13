#include <gtk/gtk.h>
#include "game.h"

Game game;
GtkWidget *grid_buttons[NUM_ROWS][NUM_COLS];

// Funkcja do aktualizacji planszy w GUI
void update_gui() {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            char text[2] = {game.grid[i][j], '\0'};
            gtk_button_set_label(GTK_BUTTON(grid_buttons[i][j]), text);
        }
    }
}

// Funkcja obsługująca kliknięcie na przycisk planszy
void on_grid_button_clicked(GtkWidget *button, gpointer data) {
    int *coords = (int *)data;
    int x = coords[0];
    int y = coords[1];

    if (game.grid[x][y] == game.computer.symbol) {
        game.grid[x][y] = '!';
        game.computer.ships--;
        game.computer.shipsSunk++;
        game.player.score += 10;
    } else {
        game.grid[x][y] = '-';
        game.attackHistory[x][y] = 'M';
    }

    update_gui();
}

// Funkcja do rozpoczęcia nowej gry w GUI
void on_new_game(GtkWidget *widget, gpointer data) {
    startNewGame(&game);
    update_gui();
}

// Inicjalizacja GUI
void start_gui(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Statki - Gra");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            int *coords = malloc(2 * sizeof(int));
            coords[0] = i;
            coords[1] = j;

            grid_buttons[i][j] = gtk_button_new_with_label(" ");
            g_signal_connect(grid_buttons[i][j], "clicked", G_CALLBACK(on_grid_button_clicked), coords);
            gtk_grid_attach(GTK_GRID(grid), grid_buttons[i][j], j, i, 1, 1);
        }
    }

    GtkWidget *button = gtk_button_new_with_label("Nowa Gra");
    g_signal_connect(button, "clicked", G_CALLBACK(on_new_game), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, NUM_ROWS, NUM_COLS, 1);

    gtk_widget_show_all(window);
    gtk_main();
}
