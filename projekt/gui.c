#include <stdio.h>
#include <stdlib.h>
#include "game.h"

// Funkcja do drukowania planszy
void printOceanMap(Game* game) {
    printf("\n           Plansza Gracza           |           Plansza Komputera\n");
    printf("  ");
    for (int i = 0; i < NUM_COLS; i++) {
        printf("%d", i);
    }
    printf("    |    ");
    for (int i = 0; i < NUM_COLS; i++) {
        printf("%d", i);
    }
    printf("\n");

    for (int i = 0; i < NUM_ROWS; i++) {
        printf("%d|", i);
        for (int j = 0; j < NUM_COLS; j++) {
            printf("%c", game->grid[i][j]);
        }
        printf("|%d|", i);
        for (int j = 0; j < NUM_COLS; j++) {
            printf("%c", game->attackHistory[i][j]);
        }
        printf("\n");
    }

    printf("  ");
    for (int i = 0; i < NUM_COLS; i++) {
        printf("%d", i);
    }
    printf("    |    ");
    for (int i = 0; i < NUM_COLS; i++) {
        printf("%d", i);
    }
    printf("\n");
}

// Funkcja do wyboru poziomu trudnosci
void chooseDifficulty(Game* game) {
    int choice;
    printf("\nWybierz poziom trudnosci:\n");
    printf("1. Latwy\n");
    printf("2. Sredni\n");
    printf("3. Trudny\n");
    scanf("%d", &choice);

    if (choice == 1) {
        game->difficulty = 1;
    } else if (choice == 2) {
        game->difficulty = 2;
    } else if (choice == 3) {
        game->difficulty = 3;
    } else {
        printf("Niepoprawny wybor, ustawiam trudnosc na latwa.\n");
        game->difficulty = 1;
    }
}

// Funkcja do rozpoczęcia gry
void startGame() {
    Game game = {{{' '}}, {{' '}}, {'@', TOTAL_SHIPS, 0, 0}, {'x', TOTAL_SHIPS, 0, 0}, 0};

    char choice;

    printf("**** Witamy w grze w statki ****\n");
    printf("1. Nowa gra\n");
    printf("2. Zakonczenie\n");
    printf("Wybierz opcje: ");
    scanf(" %c", &choice);

    if (choice == '1') {
        chooseDifficulty(&game);
        startNewGame(&game);
    } else if (choice == '2') {
        printf("Dziekujemy za gre! Zakonczono gre.\n");
    } else {
        printf("Niepoprawny wybór. Zakonczono gre.\n");
    }
}

// Funkcja do rozpoczęcia nowej gry (wywołanie funkcji z game.h)
void startNewGame(Game* game) {
    game->player.ships = TOTAL_SHIPS;
    game->computer.ships = TOTAL_SHIPS;
    game->player.shipsSunk = 0;
    game->computer.shipsSunk = 0;
    game->player.score = 0;
    game->computer.score = 0;
    game->turnCounter = 0;
    createOceanMap(game);
    deployPlayerShips(game);
    deployComputerShips(game);
    printf("\nNowa gra rozpoczęta!\n");

    while (!checkGameOver(game) && game->turnCounter < MAX_TURNS) {
        printOceanMap(game);
        playerTurn(game);
        if (checkGameOver(game)) break;
        intelligentComputerTurn(game);
        game->turnCounter++;
    }
    gameOver(game);
}

int main() {
    startGame();
    return 0;
}
