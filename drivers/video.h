#ifndef VIDEO_H
#define VIDEO_H

// Цвета текста 
#define COLOR_BLACK         0x0
#define COLOR_BLUE          0x1
#define COLOR_GREEN         0x2
#define COLOR_CYAN          0x3
#define COLOR_RED           0x4
#define COLOR_MAGENTA       0x5
#define COLOR_BROWN         0x6
#define COLOR_LIGHT_GRAY    0x7
#define COLOR_DARK_GRAY     0x8
#define COLOR_LIGHT_BLUE    0x9
#define COLOR_LIGHT_GREEN   0xA
#define COLOR_LIGHT_CYAN    0xB
#define COLOR_LIGHT_RED     0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW        0xE
#define COLOR_WHITE         0xF

// Функция-помощник для объединения цвета текста и фона
#define MAKE_COLOR(fg, bg) ((bg << 4) | fg)

// --- ВОТ ЭТО МЫ ДОБАВИЛИ ---
// Эти переменные будут общими для видео-драйвера и Шелла
extern int current_row;
extern int current_col;

// Функции драйвера
void put_char(char c, int pos, char color);
void clear_screen();
void kprint(const char *str, int row, int col, char color);
void kprint_at_cursor(const char *str, char color); // Наша новая "умная" печать
int test_vga();
void update_cursor(int row, int col);
void scroll();
void kprint_at(char *message, int row, int col, char attribute);
void kprint_int(int value, int row, int col, char attribute);

#endif