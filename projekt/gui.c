#include <gtk/gtk.h>
#include "game.h"

GtkWidget *window;
GtkWidget *grid_labels[NUM_ROWS][NUM_COLS];
GtkWidget *status_label;
GtkWidget *score_label;
GtkWidget *difficulty_combo;
GtkWidget *turn_label;
GtkWidget *message_label;
GtkWidget *button_start;
GtkWidget *button_quit;
GtkWidget *game_box;
GtkWidget *control_box;
GtkWidget *status_box;
Game game;

void updateGridDisplay(Game* game) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            char text[2] = {game->grid[i][j], '\0'};
            gtk_label_set_text(GTK_LABEL(grid_labels[i][j]), text);
        }
    }
}

void updateStatus() {
    char status_text[100];
    snprintf(status_text, sizeof(status_text), "Punkty: Gracz (%d) - Komputer (%d)", game.player.score, game.computer.score);
    gtk_label_set_text(GTK_LABEL(score_label), status_text);
}

void updateTurnLabel() {
    if (game.turnCounter % 2 == 0) {
        gtk_label_set_text(GTK_LABEL(turn_label), "Tura: Gracza");
    } else {
        gtk_label_set_text(GTK_LABEL(turn_label), "Tura: Komputera");
    }
}

void updateMessage(const char *message) {
    gtk_label_set_text(GTK_LABEL(message_label), message);
}

void onGridCellClicked(GtkWidget *widget, gpointer data) {
    int row = GPOINTER_TO_INT(data) / NUM_COLS;
    int col = GPOINTER_TO_INT(data) % NUM_COLS;

    if (game.turnCounter % 2 == 0 && game.grid[row][col] == ' ') {
        game.grid[row][col] = game.player.symbol;
        updateGridDisplay(&game);
        game.turnCounter++;
        updateTurnLabel();
        updateMessage("Gracz wykonał ruch. Czekaj na ruch komputera...");

        if (checkGameOver(&game)) {
            gameOver(&game);
            updateMessage("Koniec gry! Gracz wygrał!");
            return;
        }

        intelligentComputerTurn(&game);
        updateGridDisplay(&game);
        game.turnCounter++;
        updateTurnLabel();

        if (checkGameOver(&game)) {
            gameOver(&game);
            updateMessage("Koniec gry! Komputer wygrał!");
            return;
        }

        updateMessage("Tura zakończona, czekaj na swoją kolej.");
    }
}

void onStartNewGameButtonClicked(GtkWidget *button, gpointer user_data) {
    startNewGame(&game);
    updateStatus();
    updateTurnLabel();
    updateMessage("Nowa gra rozpoczęta!");
}

void onQuitButtonClicked(GtkWidget *button, gpointer user_data) {
    gtk_window_close(GTK_WINDOW(window));
}

void onDifficultyChanged(GtkComboBox *combo, gpointer user_data) {
    game.difficulty = gtk_combo_box_get_active(combo) + 1;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Gra w statki");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);  // Zwiększenie rozmiaru okna
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    game_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), game_box);

    // Plansza
    GtkWidget *grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(game_box), grid, TRUE, TRUE, 0);

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            grid_labels[i][j] = gtk_label_new(" ");
            gtk_widget_set_size_request(grid_labels[i][j], 60, 60);  // Zwiększenie wielkości komórek
            gtk_grid_attach(GTK_GRID(grid), grid_labels[i][j], j, i, 1, 1);
            gtk_label_set_xalign(GTK_LABEL(grid_labels[i][j]), 0.5);
            gtk_label_set_yalign(GTK_LABEL(grid_labels[i][j]), 0.5);
            gtk_widget_set_margin_bottom(grid_labels[i][j], 5);  // Dodatkowy odstęp
            gtk_widget_set_margin_right(grid_labels[i][j], 5);
            g_signal_connect(grid_labels[i][j], "button-press-event", G_CALLBACK(onGridCellClicked), GINT_TO_POINTER(i * NUM_COLS + j));
        }
    }

    // Status i kontrolki
    status_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(game_box), status_box, FALSE, FALSE, 0);

    status_label = gtk_label_new("Status: Gra w toku");
    gtk_widget_set_name(status_label, "status_label");
    gtk_box_pack_start(GTK_BOX(status_box), status_label, TRUE, TRUE, 0);

    turn_label = gtk_label_new("Tura: Gracza");
    gtk_box_pack_start(GTK_BOX(status_box), turn_label, TRUE, TRUE, 0);

    score_label = gtk_label_new("Punkty: Gracz 0 - Komputer 0");
    gtk_box_pack_start(GTK_BOX(status_box), score_label, TRUE, TRUE, 0);

    // Komunikaty
    message_label = gtk_label_new("Witaj w grze!");
    gtk_box_pack_start(GTK_BOX(game_box), message_label, FALSE, FALSE, 10);

    // Panel z przyciskami
    control_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_box_pack_start(GTK_BOX(game_box), control_box, FALSE, FALSE, 10);

    // Wybór trudności
    GtkWidget *difficulty_label = gtk_label_new("Wybierz poziom trudności:");
    gtk_box_pack_start(GTK_BOX(control_box), difficulty_label, FALSE, FALSE, 0);

    difficulty_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(difficulty_combo), "Łatwy");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(difficulty_combo), "Średni");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(difficulty_combo), "Trudny");
    gtk_box_pack_start(GTK_BOX(control_box), difficulty_combo, FALSE, FALSE, 0);
    g_signal_connect(difficulty_combo, "changed", G_CALLBACK(onDifficultyChanged), NULL);

    // Przycisk "Nowa Gra"
    button_start = gtk_button_new_with_label("Nowa Gra");
    g_signal_connect(button_start, "clicked", G_CALLBACK(onStartNewGameButtonClicked), NULL);
    gtk_box_pack_start(GTK_BOX(control_box), button_start, TRUE, TRUE, 0);

    // Przycisk "Zakończ"
    button_quit = gtk_button_new_with_label("Zakończ");
    g_signal_connect(button_quit, "clicked", G_CALLBACK(onQuitButtonClicked), NULL);
    gtk_box_pack_start(GTK_BOX(control_box), button_quit, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
