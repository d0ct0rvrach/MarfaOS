#include "snake.h"
#include "../drivers/video.h"
#include "../drivers/keyboard.h"
#include "../utils/utils.h"

// This is a game, snake. Unfortunately not in C# LMAOOO
// Below dir_u are its constants, field length etc
#define FIELD_W 40
#define FIELD_H 20
#define MAX_LEN 100 

typedef struct {
    int x;
    int y;
} Point;

static Point snake[MAX_LEN];
static int snake_len;
static Point food;
static int dir_x;
static int dir_y;
static int score = 0;

// field rendering
static void draw_field(int score) {

    clear_screen();
    
    for (int x = 0; x < FIELD_W + 2; x++) {
        put_char('#', 2 * 80 + x, 0x0A);
        put_char('#', (FIELD_H + 3) * 80 + x, 0x0A);
    }
    for (int y = 0; y < FIELD_H + 2; y++) {
        put_char('#', (y + 2) * 80 + 0, 0x0A);
        put_char('#', (y + 2) * 80 + FIELD_W + 1, 0x0A);
    }
        kprint("Score: ", 0, 43, 0x0F);
        kprint_int(score, 0, 50, 0x0E);
        kprint("Q - quit", 0, 55, 0x07);

}
// draw snake and food
static void draw_snake() {
    for (int i = 0; i < snake_len; i++) {
        char c = (i == 0) ? '@' : 'o';
        put_char(c, (snake[i].y + 3) * 80 + snake[i].x + 1, 0x0B);
    }
    put_char('*', (food.y + 3) * 80 + food.x + 1, 0x0C);
}

extern volatile unsigned int timer_ticks; // runs on ticks
// food spawn, not world spawn, food
static void spawn_food() {
    food.x = (timer_ticks * 7 + snake[0].x * 3 + 13) % FIELD_W; // two sources of randomness now
    food.y = (timer_ticks * 3 + snake[0].y * 5 + 7) % FIELD_H; 
}

void snake_game() { // main game function
    score = 0;
    snake_len = 3;
    snake[0].x = 10; snake[0].y = 5;
    snake[1].x = 9;  snake[1].y = 5;
    snake[2].x = 8;  snake[2].y = 5;
    dir_x = 1; dir_y = 0;
    spawn_food();

    while (1) {
        char c = get_char();
        if (c == 'w') { dir_x = 0;  dir_y = -1; }
        if (c == 's') { dir_x = 0;  dir_y = 1;  }
        if (c == 'a') { dir_x = -1; dir_y = 0;  }
        if (c == 'd') { dir_x = 1;  dir_y = 0;  }
        if (c == 'q') break;

        for (int i = snake_len - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0].x += dir_x;
        snake[0].y += dir_y;

        if (snake[0].x < 0 || snake[0].x >= FIELD_W || 
            snake[0].y < 0 || snake[0].y >= FIELD_H) {
            kprint("GAME OVER!", 12, 35, 0x0C);
            sleep(2000);
            break;
        }

        for (int i = 1; i < snake_len; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            kprint("GAME OVER!", 12, 35, 0x0C);
            sleep(2000);
            goto game_over;
            }
        }

        if (snake[0].x == food.x && snake[0].y == food.y) {
            if (snake_len < MAX_LEN) snake_len++;
            spawn_food();
            score++;
        }

        draw_field(score);
        draw_snake();
        sleep(200);
    }

    game_over:
    clear_screen();
}
