#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define CELL 20
#define ROWS 24
#define COLS 32

int score = 0;
int pacmanX = 15, pacmanY = 17;
int pacmanDX = 1, pacmanDY = 0;
int nextDX = 1, nextDY = 0;

int ghostX = 13, ghostY = 11;
int ghostDX = 0, ghostDY = 0;
int ghostMoveCounter = 0;
int ghost2X = 16, ghost2Y = 11;
int ghost2DX = 0, ghost2DY = 0;
int ghost2MoveCounter = 0;
int ghost3X = 15, ghost3Y = 13;
int ghost3DX = 0, ghost3DY = 0;
int ghost3MoveCounter = 0;

int accumulatedScore = 0;

const char originalMap[ROWS][COLS + 1] = {
    "############################",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#o####.#####.##.#####.####o#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##....##....##......#",
    "######.##### ## #####.######",
    "     #.##### ## #####.#     ",
    "     #.##          ##.#     ",
    "     #.## ###--### ##.#     ",
    "######.## #      # ##.######",
    "      .   #      #   .      ",
    "######.## #      # ##.######",
    "     #.## ######## ##.#     ",
    "     #.##          ##.#     ",
    "     #.## ######## ##.#     ",
    "######.## ######## ##.######",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#o..##................##..o#",
    "############################"
};

char map[ROWS][COLS + 1] = {
    "############################",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#o####.#####.##.#####.####o#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##....##....##......#",
    "######.##### ## #####.######",
    "     #.##### ## #####.#     ",
    "     #.##          ##.#     ",
    "     #.## ###--### ##.#     ",
    "######.## #      # ##.######",
    "      .   #      #   .      ",
    "######.## #      # ##.######",
    "     #.## ######## ##.#     ",
    "     #.##          ##.#     ",
    "     #.## ######## ##.#     ",
    "######.## ######## ##.######",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#o..##................##..o#",
    "############################"
};

void drawWall(int x, int y) {
    setcolor(GREEN); // Matrix-like green

    int px = x * CELL;
    int py = y * CELL;

    for (int i = 2; i < CELL; i += 4) {
        line(px + i, py, px + i, py + CELL); // vertical mini-lines
    }

    for (int i = 2; i < CELL; i += 4) {
        line(px, py + i, px + CELL, py + i); // horizontal mini-lines
    }
}

void drawGame() {
    clearviewport();

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            switch (map[y][x]) {
                case '#':
                    drawWall(x, y);
                    break;
                case '.':
                    setcolor(WHITE);
                    circle(x * CELL + CELL / 2, y * CELL + CELL / 2, 2);
                    break;
                case 'o':
                    setcolor(WHITE);
                    circle(x * CELL + CELL / 2, y * CELL + CELL / 2, 4);
                    break;
            }
        }
    }

    // Pac-Man
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(pacmanX * CELL + CELL / 2, pacmanY * CELL + CELL / 2, 8, 8);

    // Ghost 1
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(ghostX * CELL + CELL / 2, ghostY * CELL + CELL / 2, 8, 8);
    // Ghost 2
    setcolor(MAGENTA);
    setfillstyle(SOLID_FILL, MAGENTA);
    fillellipse(ghost2X * CELL + CELL / 2, ghost2Y * CELL + CELL / 2, 8, 8);
    // Ghost 3
    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(ghost3X * CELL + CELL / 2, ghost3Y * CELL + CELL / 2, 8, 8);

    // Score
    setcolor(WHITE);
    char scr[30];
    sprintf(scr, "Score: %d", score);
    outtextxy(10, 10, scr);
}

bool canMove(int x, int y) {
    return map[y][x] != '#';
}

void wrapPosition(int &x, int &y) {
    // Only wrap horizontally if the map is open (no wall)
    if (x < 0) x = COLS - 1;
    if (x >= COLS) x = 0;
    // Don't wrap vertically
}

void updatePacman() {
    int tryX = pacmanX + nextDX;
    int tryY = pacmanY + nextDY;
    wrapPosition(tryX, tryY);
    if (canMove(tryX, tryY)) {
        pacmanDX = nextDX;
        pacmanDY = nextDY;
    }
    if (pacmanDX == 0 && pacmanDY == 0) {
        int tryX2 = pacmanX + nextDX;
        int tryY2 = pacmanY + nextDY;
        wrapPosition(tryX2, tryY2);
        if (canMove(tryX2, tryY2)) {
            pacmanDX = nextDX;
            pacmanDY = nextDY;
        }
    }
    int newX = pacmanX + pacmanDX;
    int newY = pacmanY + pacmanDY;
    wrapPosition(newX, newY);
    if (canMove(newX, newY)) {
        pacmanX = newX;
        pacmanY = newY;
        if (map[newY][newX] == '.') {
            map[newY][newX] = ' ';
            score += 1;
        } else if (map[newY][newX] == 'o') {
            map[newY][newX] = ' ';
            score += 5;
        }
    } else {
        pacmanDX = 0;
        pacmanDY = 0;
    }
}

void updateGhost(int &gX, int &gY, int &gDX, int &gDY, int &moveCounter) {
    moveCounter++;
    bool needNewDir = (moveCounter % 10 == 0) || !canMove(gX + gDX, gY + gDY);
    if (needNewDir) {
        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        int valid[4], count = 0;
        for (int i = 0; i < 4; i++) {
            int nx = gX + dirs[i][0];
            int ny = gY + dirs[i][1];
            wrapPosition(nx, ny);
            if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS && canMove(nx, ny)) {
                if (!(gDX == -dirs[i][0] && gDY == -dirs[i][1])) {
                    valid[count++] = i;
                }
            }
        }
        if (count > 0) {
            int dir = valid[rand() % count];
            gDX = dirs[dir][0];
            gDY = dirs[dir][1];
        } else {
            for (int i = 0; i < 4; i++) {
                int nx = gX + dirs[i][0];
                int ny = gY + dirs[i][1];
                wrapPosition(nx, ny);
                if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS && canMove(nx, ny)) {
                    gDX = dirs[i][0];
                    gDY = dirs[i][1];
                    break;
                }
            }
        }
    }
    int nx = gX + gDX;
    int ny = gY + gDY;
    wrapPosition(nx, ny);
    if (canMove(nx, ny) && nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS) {
        gX = nx;
        gY = ny;
    }
}

bool checkGameOver() {
    return (pacmanX == ghostX && pacmanY == ghostY) ||
           (pacmanX == ghost2X && pacmanY == ghost2Y) ||
           (pacmanX == ghost3X && pacmanY == ghost3Y);
}

bool allDotsEaten() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (map[y][x] == '.' || map[y][x] == 'o') return false;
        }
    }
    return true;
}

int showPostGameMenu(bool win, int score, int accumulatedScore) {
    setactivepage(0);
    setvisualpage(0);
    clearviewport();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    setcolor(win ? GREEN : RED);
    char msg[20];
    if (win)
        strcpy(msg, "YOU WIN!");
    else
        strcpy(msg, "GAME OVER!");
    outtextxy(100, 100, msg);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    char scr[50];
    sprintf(scr, "Score: %d", score);
    outtextxy(120, 180, scr);
    sprintf(scr, "Accumulated Score: %d", accumulatedScore);
    outtextxy(80, 220, scr);
    outtextxy(100, 270, "Press ENTER to Play Again");
    outtextxy(140, 310, "Press ESC to Exit");
    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == 13) return 1; // ENTER
            if (ch == 27) return 0; // ESC
        }
        delay(50);
    }
}

void showMenu() {
    clearviewport();
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(120, 120, "PACMAN");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(100, 200, "Press ENTER to Start");
    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == 13) break; // ENTER
        }
        delay(50);
    }
    clearviewport();
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    srand(time(NULL));
    accumulatedScore = 0;
    while (1) {
        // Reset game state
        score = 0;
        pacmanX = 15; pacmanY = 17;
        pacmanDX = 1; pacmanDY = 0;
        nextDX = 1; nextDY = 0;
        ghostX = 13; ghostY = 11; ghostDX = 0; ghostDY = 0; ghostMoveCounter = 0;
        ghost2X = 16; ghost2Y = 11; ghost2DX = 0; ghost2DY = 0; ghost2MoveCounter = 0;
        ghost3X = 15; ghost3Y = 13; ghost3DX = 0; ghost3DY = 0; ghost3MoveCounter = 0;
        // Reset map
        for (int y = 0; y < ROWS; y++) {
            strcpy(map[y], originalMap[y]);
        }
        showMenu();
        int page = 0;
        int gameOver = 0;
        int win = 0;
        while (!gameOver) {
            setactivepage(page);
            setvisualpage(1 - page);
            drawGame();
            if (kbhit()) {
                char ch = getch();
                if (ch == 27) break; // ESC
                if (ch == 0 || ch == -32) {
                    ch = getch();
                    switch (ch) {
                        case 72: nextDX = 0; nextDY = -1; break;
                        case 80: nextDX = 0; nextDY = 1; break;
                        case 75: nextDX = -1; nextDY = 0; break;
                        case 77: nextDX = 1; nextDY = 0; break;
                    }
                }
            }
            updatePacman();
            updateGhost(ghostX, ghostY, ghostDX, ghostDY, ghostMoveCounter);
            updateGhost(ghost2X, ghost2Y, ghost2DX, ghost2DY, ghost2MoveCounter);
            updateGhost(ghost3X, ghost3Y, ghost3DX, ghost3DY, ghost3MoveCounter);
            if (checkGameOver()) {
                delay(1000);
                gameOver = 1;
                win = 0;
                break;
            }
            if (allDotsEaten()) {
                delay(1000);
                gameOver = 1;
                win = 1;
                break;
            }
            delay(100);
            page = 1 - page;
        }
        setactivepage(0);
        setvisualpage(0);
        accumulatedScore += score;
        int playAgain = showPostGameMenu(win, score, accumulatedScore);
        if (!playAgain) break;
    }
    closegraph();
    return 0;
}

