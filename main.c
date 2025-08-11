#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 10
#define SNAKE_MAX_LENGTH 100

typedef enum { STOP = 0, LEFT, RIGHT, UP, DOWN } Direction;

typedef struct {
    int x;
    int y;
} Coords;

typedef struct {
    Coords head;
    Coords body[SNAKE_MAX_LENGTH];
    int length;
    Direction dir;
} Snake;

bool gameOver;
int score;
Snake snake;
Coords food;
int gameSpeed = 100000;

void setupGame();
void drawGame();
void handleInput();
void updateGame();
void generateFood();

int main() {
    setupGame();

    while (!gameOver) {
        handleInput();
        updateGame();
        drawGame();
        usleep(gameSpeed);
    }

    endwin();
    printf("Game Over! Your score was: %d\n", score);

    return 0;
}

void setupGame() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    gameOver = false;
    score = 0;
    snake.dir = STOP;
    snake.head.x = WIDTH / 2;
    snake.head.y = HEIGHT / 2;
    snake.length = 1;
    snake.body[0] = snake.head;

    srand(time(NULL));
    generateFood();
}

void drawGame() {
    clear();

    for (int i = 0; i < WIDTH + 2; i++) {
        mvaddch(0, i, '#');
        mvaddch(HEIGHT + 1, i, '#');
    }
    for (int i = 0; i < HEIGHT + 2; i++) {
        mvaddch(i, 0, '#');
        mvaddch(i, WIDTH + 1, '#');
    }

    mvaddch(snake.head.y, snake.head.x, 'O');
    for (int i = 1; i < snake.length; i++) {
        mvaddch(snake.body[i].y, snake.body[i].x, 'o');
    }

    mvaddch(food.y, food.x, '*');

    mvprintw(HEIGHT + 3, 0, "Score: %d", score);
    mvprintw(HEIGHT + 4, 0, "Press 'x' to exit");

    refresh();
}

void handleInput() {
    int key = getch();
    switch (key) {
        case KEY_LEFT:
            if (snake.dir != RIGHT) snake.dir = LEFT;
            break;
        case KEY_RIGHT:
            if (snake.dir != LEFT) snake.dir = RIGHT;
            break;
        case KEY_UP:
            if (snake.dir != DOWN) snake.dir = UP;
            break;
        case KEY_DOWN:
            if (snake.dir != UP) snake.dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
    }
}

void updateGame() {
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }
    snake.body[0] = snake.head;

    switch (snake.dir) {
        case LEFT:
            snake.head.x--;
            break;
        case RIGHT:
            snake.head.x++;
            break;
        case UP:
            snake.head.y--;
            break;
        case DOWN:
            snake.head.y++;
            break;
        default:
            break;
    }

    if (snake.head.x <= 0 || snake.head.x >= WIDTH + 1 || snake.head.y <= 0 || snake.head.y >= HEIGHT + 1) {
        gameOver = true;
    }

    for (int i = 1; i < snake.length; i++) {
        if (snake.head.x == snake.body[i].x && snake.head.y == snake.body[i].y) {
            gameOver = true;
        }
    }

    if (snake.head.x == food.x && snake.head.y == food.y) {
        score += 10;
        snake.length++;
        generateFood();
    }
}

void generateFood() {
    bool foodOnSnake = true;
    while(foodOnSnake){
        food.x = rand() % WIDTH + 1;
        food.y = rand() % HEIGHT + 1;
        foodOnSnake = false;
        for(int i = 0; i < snake.length; i++){
            if(food.x == snake.body[i].x && food.y == snake.body[i].y){
                foodOnSnake = true;
                break;
            }
        }
    }
}