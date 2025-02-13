#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// Funkcja do inicjowania planszy
void createOceanMap(Game* game) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            game->grid[i][j] = ' ';
            game->attackHistory[i][j] = ' ';
        }
    }
}

// Funkcja do drukowania plansz obok siebie
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

// Funkcja do rozmieszczania statkow gracza
void deployPlayerShips(Game* game) {
    int x, y;
    printf("\nRozmiesc swoje statki:\n");
    for (int i = 1; i <= TOTAL_SHIPS;) {
        printf("Wprowadz wspolrzedna X dla statku nr %d: ", i);
        scanf("%d", &x);
        printf("Wprowadz wspolrzedna Y dla statku nr %d: ", i);
        scanf("%d", &y);

        if (x >= 0 && x < NUM_ROWS && y >= 0 && y < NUM_COLS && game->grid[x][y] == ' ') {
            game->grid[x][y] = game->player.symbol;
            i++;
        } else if (game->grid[x][y] == game->player.symbol) {
            printf("Nie mozesz umiescic dwoch statkow w tym samym miejscu.\n");
        } else {
            printf("Nie mozesz wprowadzic statkow poza mape.\n");
        }
    }
    printOceanMap(game);
}

// Funkcja do rozmieszczania statkow komputera
void deployComputerShips(Game* game) {
    srand(time(NULL)); // Inicjalizacja generatora liczb losowych
    int x, y;
    printf("\nKomputer rozstawia swoje statki:\n");
    for (int i = 1; i <= TOTAL_SHIPS;) {
        x = rand() % NUM_ROWS;
        y = rand() % NUM_COLS;

        if (game->grid[x][y] == ' ') {
            game->grid[x][y] = game->computer.symbol;
            i++;
        }
    }
    printOceanMap(game);
}

// Funkcja do tury gracza
void playerTurn(Game* game) {
    int x, y;
    printf("\nTURA GRACZA\n");
    do {
        printf("Wprowadz wspolrzedna X: ");
        scanf("%d", &x);
        printf("Wprowadz wspolrzedna Y: ");
        scanf("%d", &y);

        if (x >= 0 && x < NUM_ROWS && y >= 0 && y < NUM_COLS) {
            if (game->grid[x][y] == game->computer.symbol) {
                printf("Bum! Zatopiles statek komputera!\n");
                game->grid[x][y] = '!';
                game->computer.ships--;
                game->computer.shipsSunk++;
                game->player.score += 10;  // Zwiekszenie punktow za zatopienie statku
                break;
            } else if (game->grid[x][y] == game->player.symbol) {
                printf("O nie, zatopiles swoj wlasny statek!\n");
                game->grid[x][y] = 'x';
                game->player.ships--;
                game->player.shipsSunk++;
                game->computer.score += 5; // Komputer zdobywa punkty za trafienie
                break;
            } else {
                printf("Pudlo!\n");
                game->grid[x][y] = '-';
                game->attackHistory[x][y] = 'M'; // 'M' - missed
                break;
            }
        } else {
            printf("Nie mozesz wprowadzic wspolrzednych poza mape.\n");
        }
    } while (1);
}

// Funkcja do tury komputera
void computerTurn(Game* game) {
    int x, y;
    printf("\nTURA KOMPUTERA\n");
    do {
        x = rand() % NUM_ROWS;
        y = rand() % NUM_COLS;

        if (game->grid[x][y] == game->player.symbol) {
            printf("Komputer zatopil twój statek!\n");
            game->grid[x][y] = 'x';
            game->player.ships--;
            game->player.shipsSunk++;
            game->computer.score += 10;  // Komputer zdobywa punkty za trafienie
            break;
        } else if (game->grid[x][y] == game->computer.symbol) {
            printf("Komputer zatopil swoj wlasny statek.\n");
            game->grid[x][y] = '!';
            break;
        } else {
            printf("Komputer nie trafil.\n");
            game->grid[x][y] = '-';
            game->attackHistory[x][y] = 'M'; // 'M' - missed
            break;
        }
    } while (1);
}

// Funkcja do tury komputera z poziomem trudnosci
void intelligentComputerTurn(Game* game) {
    int x, y;
    static int lastX = -1, lastY = -1;
    static int direction = 0;

    printf("\nTURA KOMPUTERA (INTELIGENTNA)\n");

    if (game->difficulty == 3) { // Trudny poziom
        if (lastX != -1 && lastY != -1) {
            if (direction == 0) {
                // Atak w lewo
                if (lastY > 0) {
                    y = lastY - 1;
                    x = lastX;
                } else {
                    direction = 1;
                    y = lastY + 1;
                    x = lastX;
                }
            } else if (direction == 1) {
                // Atak w prawo
                if (lastY < NUM_COLS - 1) {
                    y = lastY + 1;
                    x = lastX;
                } else {
                    direction = 0;
                    y = lastY - 1;
                    x = lastX;
                }
            }

            if (game->grid[x][y] == game->player.symbol) {
                printf("Komputer trafil twój statek!\n");
                game->grid[x][y] = 'x';
                game->player.ships--;
                game->player.shipsSunk++;
                game->computer.score += 10;  // Komputer zdobywa punkty
            } else if (game->grid[x][y] == ' ') {
                printf("Komputer nie trafil.\n");
                game->grid[x][y] = '-';
                game->attackHistory[x][y] = 'M'; // 'M' - missed
            }
        } else {
            // Pierwszy atak komputera - losowy
            x = rand() % NUM_ROWS;
            y = rand() % NUM_COLS;

            if (game->grid[x][y] == game->player.symbol) {
                printf("Komputer trafil twój statek!\n");
                game->grid[x][y] = 'x';
                game->player.ships--;
                game->player.shipsSunk++;
                game->computer.score += 10;
            } else if (game->grid[x][y] == ' ') {
                printf("Komputer nie trafil.\n");
                game->grid[x][y] = '-';
                game->attackHistory[x][y] = 'M'; // 'M' - missed
            }
        }

        lastX = x;
        lastY = y;
    } else {
        computerTurn(game);
    }
}

// Funkcja sprawdzajaca, czy gra zakonczona
int checkGameOver(Game* game) {
    if (game->player.ships == 0 || game->computer.ships == 0) {
        return 1; // Gra zakonczona
    }
    return 0; // Gra trwa
}

// Funkcja konczaca gre
void gameOver(Game* game) {
    printf("\nTwoje statki: %d | Statki komputera: %d\n", game->player.ships, game->computer.ships);
    if (game->player.ships > 0 && game->computer.ships <= 0) {
        printf("Hurra! Wygrales! Punkty: %d\n", game->player.score);
    } else if (game->computer.ships > 0 && game->player.ships <= 0) {
        printf("Niestety, przegrales. Punkty komputera: %d\n", game->computer.score);
    }
}

// Funkcja do rozpoczecia nowej gry
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
    printf("\nNowa gra rozpoczêta!\n");

    while (!checkGameOver(game) && game->turnCounter < MAX_TURNS) {
        printOceanMap(game);
        playerTurn(game);
        if (checkGameOver(game)) break;
        intelligentComputerTurn(game);
        game->turnCounter++;
    }
    gameOver(game);
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

int main() {
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


    return 0;
}
