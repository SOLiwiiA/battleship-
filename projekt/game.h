#ifndef GAME_H
#define GAME_H

#define NUM_ROWS 10
#define NUM_COLS 10
#define TOTAL_SHIPS 5
#define MAX_TURNS 100

// Struktura reprezentujaca gracza
typedef struct {
    char symbol;            // Symbol gracza (gracz: '@', komputer: 'x')
    int ships;              // Liczba statkow
    int shipsSunk;          // Liczba zatopionych statkow
    int score;              // Punkty gracza
} Player;

// Struktura reprezentujaca gre
typedef struct {
    char grid[NUM_ROWS][NUM_COLS];  // Plansza
    char attackHistory[NUM_ROWS][NUM_COLS]; // Historia atakow
    Player player;                   // Gracz
    Player computer;                 // Komputer
    int turnCounter;                 // Licznik tur
    int difficulty;                  // Poziom trudnosci (1 - latwy, 2 - sredni, 3 - trudny)
} Game;

// Funkcje do obsługi gry
void createOceanMap(Game* game);
void printOceanMap(Game* game);
void deployPlayerShips(Game* game);
void deployComputerShips(Game* game);
void playerTurn(Game* game);
void computerTurn(Game* game);
void intelligentComputerTurn(Game* game);
int checkGameOver(Game* game);
void gameOver(Game* game);
void startNewGame(Game* game);
void chooseDifficulty(Game* game);

#endif // GAME_H
