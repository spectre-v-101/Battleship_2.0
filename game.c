#include <stdio.h>
#include <stdlib.h>///for rand() and srand()
#include <time.h>
#include <windows.h>   //for clear screen
#include <mmsystem.h> // For playing music
#include <conio.h>  // For _getch()
#include <math.h>
#define SIZE 18
#define SHIP_COUNT 14
int playerTorpedoUsed = 0;
int aiTorpedoUsed = 0;
int playerTorpedoCooldown = 5;
int aiTorpedoCooldown = 5;


char playerBoard[SIZE][SIZE];
char enemyBoard[SIZE][SIZE];
char playerAttackBoard[SIZE][SIZE];
char enemyAttackBoard[SIZE][SIZE];

int shipSizes[SHIP_COUNT] = { 3,3,3,4,4,4,4,5,5,5,5,6,6,6 };
char shipSymbols[SHIP_COUNT] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N'};
int playerShipHealth[SHIP_COUNT];
int enemyShipHealth[SHIP_COUNT];




#define RESET   "\x1b[0m"
#define BLUE    "\x1b[34m"
#define RED2     "\x1b[41m"
#define RED "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define CYAN    "\x1b[36m"
#define GRAY    "\x1b[90m"
#define GREEN1  "\x1b[104m"
#define BGYELLOW "\x1b[103m"
#define FGWHITE "\x1b[97m"
#define RED1     "\x1b[91m"
#define BGGREY "\x1b[47m"
#define BLACK "\x1b[30m"
#define BGGREEN "\x1b[42m"
//#define GREEN1 "\x1b[42m"

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | 0x0004);
    }
}
void sonarPing() {
    Beep(600, 100);  // Ping sound
    Sleep(100);
    Beep(800, 120);  // Echo
    Sleep(200);
    Beep(500, 80);
}





void showVictoryScreen(int playerWon) {
    for (int i = 0; i < 3; i++) {
        system("cls");
       // printf("\x1b[5;%dm", playerWon ? 42 : 41); // blink + background color

        if (playerWon) {
           // PlaySound(TEXT("victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
            printf("\n\n");
            printf(RESET"                      ");
            printf(GREEN "  ============================================\n"RESET);
            printf("                      ");
            printf(GREEN"  |              YOU WIN!                    |\n"RESET);
            printf("                      ");
            printf(GREEN"  |    You destroyed the enemy fleet!        |\n"RESET);
            printf("                      ");
            printf(GREEN"  ============================================\n" RESET);
        } else {
            //PlaySound(TEXT("defeat.wav"), NULL, SND_FILENAME | SND_ASYNC);
            printf("\n\n");
            printf(RESET"                      ");
            printf(RED "  ============================================\n"RESET);
            printf("                      ");
            printf(RED"  |              YOU LOST                    |\n"RESET);
            printf("                      ");
            printf(RED"  |     Your fleet has been destroyed!       |\n"RESET);
            printf("                      ");
            printf(RED"  ============================================\n" RESET);
        }

        Sleep(800);
    }

    printf(RESET);
    printf("\n\nPress any key to exit...");
    getch();
}


void showIntroScreen() {
    system("cls");

    
    printf("\n\n\n\n\n\n\n\n");
    printf("                                     ");

    printf(BGYELLOW GREEN);
    printf("              ======================================               \n");
    printf(RESET"                                     ");
    printf(BGYELLOW BLACK);
    printf("              ||                                   ||              \n");
    printf(RESET"                                     ");
    printf(BGYELLOW BLACK);

    printf("              ||          BATTLESHIP ELITE         ||              \n");
    printf(RESET"                                     ");
    printf(BGYELLOW BLACK);

    printf("              ||        Naval Strike Simulator     ||              \n");
    printf(RESET"                                     ");
    printf(BGYELLOW BLACK);
    printf("              ||                                   ||              \n");
    printf(RESET"                                     ");
    printf(BGYELLOW GREEN);
    printf("              ======================================               \n");
    printf(RESET);

    printf("\n\n");
    printf("                                       ");
    printf(RED2 YELLOW "\x1b[1m\x1b[5m              >> Press any key to deploy fleet <<              " RESET "\n\n");
    while (!_kbhit()) {
        Beep(600, 150);   // First sonar ping
        Sleep(100);
        Beep(800, 150);   // Echo effect
        Sleep(2000);       // Pause between pings
    }
    _getch(); // Wait for key press
    system("cls");
}
#define SIZE2 21
#define CLEAR_SCREEN system("cls")
// Simple circular radar sweep pattern
void showRadarIntro() {
    char radar[SIZE2][SIZE2];
    int center = SIZE2 / 2;

    srand(time(NULL));
    CLEAR_SCREEN;

    while (!_kbhit()) {
        // Initialize radar with blank green dots
        for (int i = 0; i < SIZE2; i++)
            for (int j = 0; j < SIZE2; j++)
                radar[i][j] = '.';

        // Random blips (enemy signals)
        for (int b = 0; b < 6; b++) {
            int x = rand() % SIZE2;
            int y = rand() % SIZE2;
            if ((x - center) * (x - center) + (y - center) * (y - center) <= center * center)
                radar[x][y] = '*';
        }

        // Radar sweep effect (rotating line)
        static int angle = 0;
        for (int r = 0; r < center; r++) {
            int x = center + (int)(r * cos(angle * 3.14159 / 180));
            int y = center + (int)(r * sin(angle * 3.14159 / 180));
            if (x >= 0 && x < SIZE2 && y >= 0 && y < SIZE2)
                radar[x][y] = '|';
        }

        angle = (angle + 15) % 360; // Rotate sweep

        // Print radar
        printf(BGGREEN BLACK);
        printf("\n\n                                                       SCANNING SECTOR...                                                           \n\n" RESET);
        for (int i = 0; i < SIZE; i++) {
            printf("                                 ");
            for (int j = 0; j < SIZE2; j++) {
                char c = radar[i][j];
                if (c == '*') printf(RED " * " RESET);  // enemy blip
                else if (c == '|') printf(RESET " | "); // sweep line
                else printf(GREEN " . " RESET);         // background
            }
            printf("\n");
        }

        Beep(700, 100); // sonar ping
        Sleep(150);     // flicker time
        CLEAR_SCREEN;
    }

    _getch(); // Wait for key press
    CLEAR_SCREEN;
}

typedef struct {
    int x, y, dir;
} Target;

Target hitStack[SIZE * SIZE];
int stackTop = -1;

void pushTarget(int x, int y, int dir) {
    if (stackTop < SIZE * SIZE - 1) {
        hitStack[++stackTop] = (Target){x, y, dir};
    }
}

int popTarget(Target *t) {
    if (stackTop >= 0) {
        *t = hitStack[stackTop--];
        return 1;
    }
    return 0;
}
void sonarSweep() {
    int freqs[] = {400, 450, 500, 550, 600, 650, 700, 750, 800};
    for (int i = 0; i < 9; i++) {
        Beep(freqs[i], 40); // rising sweep
        Sleep(10);
    }
    //Sleep(30);
    Beep(900, 70); // final ping
}

void printBoards(); // Forward declaration
void animateTorpedo(int isRow, int index, int isPlayerView) {
    //showRadarIntro();
    char tempBoard[SIZE][SIZE];
    char (*board)[SIZE] = isPlayerView ? enemyBoard : playerBoard; // Actual board being hit
    
    for (int frame = 0; frame < SIZE; frame++) {
        sonarSweep();
        // Copy and conditionally mask the board
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                char cell = board[i][j];

                if (isPlayerView) {
                    // Player viewing enemy board: hide unhit ships
                    if (cell == 'X' || cell == 'O' || cell == '.')
                        tempBoard[i][j] = cell;
                    else
                        tempBoard[i][j] = '.';  // Hide unknown ship
                } else {
                    // Enemy attacking player: show everything
                    tempBoard[i][j] = cell;
                }
            }
        }

        // Apply torpedo trail
        for (int k = 0; k <= frame; k++) {
            int x = isRow ? index : k;
            int y = isRow ? k : index;

            if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
                if (k == frame)
                    tempBoard[x][y] = '>';  // Tip
                else
                    tempBoard[x][y] = '-';  // Trail
            }
        }

        // Print animation frame
        system("cls");
        printf(GRAY "           Special Torpedo in Action...\n\n" RESET);

        for (int i = 0; i < SIZE; i++) {
            printf("           ");
            for (int j = 0; j < SIZE; j++) {
                char c = tempBoard[i][j];
                if (c == '>') printf(RED "*" RESET);
                else if (c == '-') printf("# ");
                else if (c == 'X') printf(RED "X " RESET);
                else if (c == 'O') printf(YELLOW "O " RESET);
                else if (c == '.') printf(BLUE ". " RESET);
                else printf(GREEN "%c " RESET, c);
            }
            printf("\n");
        }

        //Sleep(30);
    }

    system("cls");
    printBoards();  // Show full board after animation
}





void fireTorpedo(char board[SIZE][SIZE], char attackBoard[SIZE][SIZE], int shipHealth[SHIP_COUNT], int isRow, int index, int isPlayer) {
    int hitMade = 0;
    int firstHitX = -1, firstHitY = -1;
    animateTorpedo(isRow, index, isPlayer);

    //animateTorpedo(isRow,index);
    if (isRow) {
        for (int j = 0; j < SIZE; j++) {
            if (attackBoard[index][j] == 'X' || attackBoard[index][j] == 'O') continue;

            if (board[index][j] >= 'A' && board[index][j] <= 'N') {
                attackBoard[index][j] = 'X';
                char hitSymbol = board[index][j];
                for (int k = 0; k < SHIP_COUNT; k++) {
                    if (shipSymbols[k] == hitSymbol) {
                        shipHealth[k]--;
                        break;
                    }
                }
                board[index][j] = 'X';
                if (!hitMade) {
                    firstHitX = index;
                    firstHitY = j;
                }
                hitMade = 1;
            } else {
                attackBoard[index][j] = 'O';
                if (hitMade) break; // stop after hitting a miss post-hit
            }
        }
    } else {
        for (int i = 0; i < SIZE; i++) {
            if (attackBoard[i][index] == 'X' || attackBoard[i][index] == 'O') continue;

            if (board[i][index] >= 'A' && board[i][index] <= 'N') {
                attackBoard[i][index] = 'X';
                char hitSymbol = board[i][index];
                for (int k = 0; k < SHIP_COUNT; k++) {
                    if (shipSymbols[k] == hitSymbol) {
                        shipHealth[k]--;
                        break;
                    }
                }
                board[i][index] = 'X';
                if (!hitMade) {
                    firstHitX = i;
                    firstHitY = index;
                }
                hitMade = 1;
            } else {
                attackBoard[i][index] = 'O';
                if (hitMade) break; // stop after hitting a miss post-hit
            }
        }
    }

    if (hitMade) {
        if (isPlayer) {
            printf(GREEN "           Special Torpedo HIT multiple cells!\n" RESET);
            Beep(1300, 600);
        } else {
            printf(RED "           AI used Special Torpedo and HIT multiple cells!\n" RESET);
            Beep(1300, 600);
            for (int d = 0; d < 4; d++) {
                pushTarget(firstHitX, firstHitY, d);
            }
        }
    } else {
        if (isPlayer) {
            printf(YELLOW "           Special Torpedo Found No Ships , Just Air!\n" RESET);
            Beep(600, 300);
        } else {
            printf(RED "           AI's Special Torpedo found no ships.\n" RESET);
            Beep(600, 300);
        }
    }

    Sleep(1500);
}







void initializeBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = '.';
}

int isValidPlacement(char board[SIZE][SIZE], int x, int y, int size, int horizontal) {
    if (horizontal && y + size > SIZE) return 0;
    if (!horizontal && x + size > SIZE) return 0;
    for (int i = 0; i < size; i++)
        if (board[x + (horizontal ? 0 : i)][y + (horizontal ? i : 0)] != '.') return 0;
    return 1;
}

void randomlyPlaceShips(char board[SIZE][SIZE], int shipHealth[SHIP_COUNT]) {
    for (int i = 0; i < SHIP_COUNT; i++) {
        int x, y, horizontal;
        do {
            x = rand() % SIZE;
            y = rand() % SIZE;
            horizontal = rand() % 2;
        } while (!isValidPlacement(board, x, y, shipSizes[i], horizontal));
        for (int j = 0; j < shipSizes[i]; j++) {
            board[x + (horizontal ? 0 : j)][y + (horizontal ? j : 0)] = shipSymbols[i];
        }
        shipHealth[i] = shipSizes[i];
    }
}

void printBoards() {
    system("cls");
    printf(CYAN "\x1b[1m\x1b[5m\n                          ======================== BATTLESHIP GAME ===============================\n\n" RESET);
    printf("              ");
    for (int i = 0; i < SHIP_COUNT; i++)
        printf(GREEN "%c: %d  " RESET, shipSymbols[i], playerShipHealth[i]);
    printf("\n\n");
    printf("                          Player Board                                    Enemy Board\n\n");
    printf(GRAY "               A B C D E F G H I J K L M N O P Q R        A B C D E F G H I J K L M N O P Q R      Enemy Health\n" RESET);
    for (int i = 0; i < SIZE; i++) {
        printf(GRAY "            %2d " RESET, i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (playerBoard[i][j] >= 'A' && playerBoard[i][j] <= 'N') printf(GREEN "%c " RESET, playerBoard[i][j]);
            else if (enemyAttackBoard[i][j] == 'X') printf(RED "X " RESET);
            else if (enemyAttackBoard[i][j] == 'O') printf(YELLOW "O " RESET);
            else printf(BLUE ". " RESET);
        }
        printf("    ");
        printf(GRAY "%2d " RESET, i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (playerAttackBoard[i][j] == 'X') printf(RED "X " RESET);
            else if (playerAttackBoard[i][j] == 'O') printf(YELLOW "O " RESET);
            else printf(BLUE ". " RESET);
        }
        if (i < SHIP_COUNT) printf(RED "   %c: %d" RESET, shipSymbols[i], enemyShipHealth[i]);
        printf("\n");
    }
    printf(CYAN "\x1b[1m\x1b[5m\n                         ========================================================================\n\n" RESET);
}





void processAttack(char board[SIZE][SIZE], char attackBoard[SIZE][SIZE], int shipHealth[SHIP_COUNT], char column, int row) {
    int x = row - 1, y = column - 'A';
    if (attackBoard[x][y] == 'X' || attackBoard[x][y] == 'O') 
    {printf(RED "             GRID ALREADY ATTACKED , CAN'T BE ATTACKED AGAIN\n");
    return;
    }
    int hit=0;
    if (board[x][y] >= 'A' && board[x][y] <= 'N') {
        attackBoard[x][y] = 'X';
        char hitSymbol = board[x][y];
        for (int i = 0; i < SHIP_COUNT; i++) {
            if (shipSymbols[i] == hitSymbol) {
                shipHealth[i]--;
                break;
            }
        }
        hit=1;
        board[x][y] = 'X';
        
        printf(GREEN"                                Hit!\n"RESET);
        Beep(1200,700);
        
    } else {
        attackBoard[x][y] = 'O';
    
        printf(YELLOW"                               Miss!\n"RESET);
        Beep(600,300);
    }
}
void playerTurn() {
    char column;
    int row;

    if (playerTorpedoCooldown >= 5) {
        char choice;
        printf("\n\n           Use Special Torpedo? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            char mode;
            printf("           Fire on row or column? (r/c): ");
            scanf(" %c", &mode);
            int index;
            if (mode == 'r') {
                printf("           Enter row number (1-%d): ", SIZE);
                scanf("%d", &index);
                index--;
                if (index >= 0 && index < SIZE) {
                    fireTorpedo(enemyBoard, playerAttackBoard, enemyShipHealth, 1, index, 1);
                    playerTorpedoCooldown = 0;  // Reset cooldown
                    return;
                }
            } else if (mode == 'c') {
                printf("           Enter column letter (A-%c): ", 'A' + SIZE - 1);
                scanf(" %c", &column);
                index = column - 'A';
                if (index >= 0 && index < SIZE) {
                    fireTorpedo(enemyBoard, playerAttackBoard, enemyShipHealth, 0, index, 1);
                    playerTorpedoCooldown = 0;  // Reset cooldown
                    return;
                }
            }
            printf("           Invalid torpedo input. Skipping.\n");
        }
    } else {
        printf(YELLOW "\n           Torpedo unavailable. %d turns left.\n" RESET, 5 - playerTorpedoCooldown);
    }

    do {
        printf("\n\n             Enter your attack (e.g., A5): ");
        scanf(" %c%d", &column, &row);
        if(column < 'A' || column >= 'A' + SIZE || row < 1 || row > SIZE){
            printf("                    INVALID INPUT\n");
            Sleep(1000);
            system("cls");
            printBoards();
        }
    } while (column < 'A' || column >= 'A' + SIZE || row < 1 || row > SIZE);

    processAttack(enemyBoard, playerAttackBoard, enemyShipHealth, column, row);
}





void aiTurn() {
    int x, y;
    Target t;
    int dx[] = {-1, 1, 0, 0}; // Up, Down, Left, Right
    int dy[] = {0, 0, -1, 1};

    // The will Only use torpedo if in hunt mode (no targets to pursue)
    if (aiTorpedoCooldown >= 5 && stackTop == -1) {
        int chance = rand() % 100;
        if (chance < 70) {  // 70% chance to use torpedo
            int isRow = rand() % 2;
            int index = rand() % SIZE;
            fireTorpedo(playerBoard, enemyAttackBoard, playerShipHealth, isRow, index, 0);
            aiTorpedoCooldown = 0;
            return;
        }
    }

    // Pursuit mode
    while (popTarget(&t)) {
        int nx = t.x + dx[t.dir], ny = t.y + dy[t.dir];
        if (nx >= 0 && ny >= 0 && nx < SIZE && ny < SIZE && enemyAttackBoard[nx][ny] == '.') {
            x = nx; y = ny;

            char resultBefore = playerBoard[x][y];
            processAttack(playerBoard, enemyAttackBoard, playerShipHealth, 'A' + y, x + 1);
            printf(RED "                           AI attacked %c%d\n" RESET, 'A' + y, x + 1);
            Sleep(1000);

            if (resultBefore >= 'A' && resultBefore <= 'N') {
                // Continue in same direction
                pushTarget(x, y, t.dir);
            } else {
                // Try all other directions from original hit
                for (int d = 0; d < 4; d++) {
                    if (d == t.dir) continue;
                    pushTarget(t.x, t.y, d);
                }
            }
            return;
        }
    }

    // Hunt mode
    do {
        x = rand() % SIZE;
        y = rand() % SIZE;
    } while ((x + y) % 2 != 0 || enemyAttackBoard[x][y] != '.');

    char resultBefore = playerBoard[x][y];
    processAttack(playerBoard, enemyAttackBoard, playerShipHealth, 'A' + y, x + 1);
    printf(RED "                           AI attacked %c%d\n" RESET, 'A' + y, x + 1);
    Sleep(1000);

    if (resultBefore >= 'A' && resultBefore <= 'N') {
        // Add all directions for fresh hit
        for (int d = 0; d < 4; d++) {
            pushTarget(x, y, d);
        }
    }
}






int isFleetDestroyed(int shipHealth[SHIP_COUNT]) {
    for (int i = 0; i < SHIP_COUNT; i++)
        if (shipHealth[i] > 0) return 0;
    return 1;
}

int main() {
    srand(time(NULL));
    enableANSI();
    PlaySound(TEXT("nano.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    showRadarIntro();
    showIntroScreen();
    initializeBoard(playerBoard);
    initializeBoard(enemyBoard);
    initializeBoard(playerAttackBoard);
    initializeBoard(enemyAttackBoard);
    randomlyPlaceShips(playerBoard, playerShipHealth);
    randomlyPlaceShips(enemyBoard, enemyShipHealth);

    while (1) {
        printBoards();
        playerTurn();
        if (isFleetDestroyed(enemyShipHealth)) {
            printBoards();
            showVictoryScreen(1);
            
            break;
        }

        aiTurn();
        

        if (isFleetDestroyed(playerShipHealth)) {
            printBoards();
            showVictoryScreen(0);
            
            break;
        }
        playerTorpedoCooldown++;
        aiTorpedoCooldown++;
        Sleep(1000);
    }

   // PlaySound(NULL, NULL, 0);
    return 0;
}
