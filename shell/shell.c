#include "shell.h"
#include "../utils/utils.h"
#include "../drivers/video.h"
#include "../drivers/keyboard.h"
#include "../drivers/io.h"
#include "../drivers/speaker.h"
#include "../cpu/task.h"
#include "../drivers/VGA.h"
#include "../drivers/acpi.h"
#include "../apps/snake.h"
#include "../boot/ramdisk.h"

extern int current_row;
extern int current_col;
extern task_t *ready_queue;
extern task_t *current_task;
extern unsigned int total_system_ticks;

#define HISTORY_SIZE 10 //хранение последних команд

static char history[HISTORY_SIZE][80]; //Массив из10 строк по 80 каждая
static int history_count = 0; //сохраненые комнды точнее количество сохраненых 
static int history_index = -1; // показатель команды при нажатии стрлки


void handle_command(char *input, int *row) {
    if (*row < 24) (*row)++; else scroll();

    if (strcmp(input, "help") == 0) {
        kprint("Available: help, clear, whoami, reboot, beep, gui, tasks, timesys, echo, off",  *row, 0, MAKE_COLOR(COLOR_LIGHT_GRAY, COLOR_BLACK));
        if (*row < 24) (*row)++; else scroll();
        kprint(" dir, cat, snake",    *row, 0, MAKE_COLOR(COLOR_LIGHT_GRAY, COLOR_BLACK));
    }
    else if (strcmp(input, "tasks") == 0) {
        kprint("PID   CPU USAGE   TICKS", *row, 0, 0x0E);
        if (*row < 24) (*row)++; else scroll();

        task_t *temp = ready_queue;
        if (temp == 0) {
            kprint("No tasks found.", *row, 0, 0x0C);
        } else {
            do {
                unsigned int usage = 0;
                if (total_system_ticks > 0) {
                    usage = (temp->ticks * 100) / total_system_ticks;
                }

                kprint("ID:", *row, 0, 0x07);
                kprint_int(temp->id, *row, 4, 0x0F);
                kprint_int(usage, *row, 10, 0x0A);
                kprint("%", *row, 13, 0x0A);
                kprint_int(temp->ticks, *row, 18, 0x08);

                if (*row < 24) (*row)++; else scroll();
                temp = temp->next;
            } while (temp != ready_queue);
        }
    }
    else if (strcmp(input, "clear") == 0) {
        clear_screen();
        *row = -1;
    }
    else if (strcmp(input, "gui") == 0) {
        kprint("Switching to Graphics Mode...", *row, 0, 0x0A);
        wait(10);
        set_mode_13h();
        clear_screen_graphics(2);
        put_pixel(160, 100, 15);
    }
    else if (strcmp(input, "whoami") == 0) {
        kprint("MarfaOS v0.5 d0ct0rvrach", *row, 0, MAKE_COLOR(COLOR_LIGHT_CYAN, COLOR_BLACK));
    }
    else if (strcmp(input, "reboot") == 0) {
        kprint("System rebooting...", *row, 0, MAKE_COLOR(COLOR_LIGHT_RED, COLOR_BLACK));
        outb(0x64, 0xFE);
    }
    else if (strcmp(input, "beep") == 0) {
        beep();
    }
    else if (strcmp(input, "snake") == 0) {
    snake_game();
    current_row = 0;
    current_col = 0;
    }
   else if (strcmp(input, "crash") == 0) { // вызывает фСОД для теста, посмотрите) 
    asm volatile("int $0");
    }
    else if (input[0] == '\0') {
        (*row)--;
    }
    else if (strncmp(input, "echo", 4) == 0) {
    char *arg = skip_command(input);
    kprint(arg, *row, 0, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
    }
    else if (strcmp(input, "timesys") == 0) {
    unsigned int seconds = total_system_ticks / 100;
    kprint("timesys: ", *row, 0, MAKE_COLOR(COLOR_LIGHT_CYAN, COLOR_BLACK));
    kprint_int(seconds, *row, 9, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
    kprint("s", *row, 13, MAKE_COLOR(COLOR_LIGHT_CYAN, COLOR_BLACK));
    }
    else if (strcmp(input, "off") == 0) {
    kprint("System shutting down...", *row, 0, MAKE_COLOR(COLOR_LIGHT_RED, COLOR_BLACK));
    off();
    }
    else if (strcmp(input, "dir") == 0) {
    ramdisk_dir();
    }
    else if (strncmp(input, "cat", 3) == 0) {
    char *arg = skip_command(input);
    ramdisk_cat(arg);
    }
    else {
        kprint("Bad command! Type 'help'", *row, 0, MAKE_COLOR(COLOR_LIGHT_RED, COLOR_BLACK));
    }

    if (*row < 24) (*row)++; else scroll();
    kprint("mar@ ", *row, 0, MAKE_COLOR(COLOR_LIGHT_GREEN, COLOR_BLACK));
    update_cursor(*row, 18);
}

void run_shell() {
    current_row = 0;
    current_col = 0;

    kprint("MARFA OS v1.0.", current_row, 0, 0x0F);
    current_row++;
    kprint("mar@", current_row, 0, 0x0A);

    char input_buffer[80];
    int char_idx = 0;  

    for(int i=0; i<80; i++) input_buffer[i] = 0;

    while(1) {
        // Ожидание
        current_task->state = STATE_WAITING;

        char c = get_char();
        if (c != 0) {
            current_task->state = STATE_RUNNING;

            if (c == '\n') {
                input_buffer[char_idx] = '\0';
                if (char_idx > 0) { // не сохранять пустые команды
                for(int i = 0; i < char_idx + 1; i++)
                history[history_count % HISTORY_SIZE][i] = input_buffer[i]; // буфер кольцо
                history_count++; // + счетчик
                 history_index = -1; // сброс пизиции листания
                }
                handle_command(input_buffer, &current_row);
                for(int i=0; i<80; i++) input_buffer[i] = 0;
                char_idx = 0;
            }

            else if (c == '\b' && char_idx > 0) {
                char_idx--;
                input_buffer[char_idx] = 0;
                put_char(' ', current_row * 80 + 4 + char_idx, 0x0F);
                update_cursor(current_row, 16 + char_idx);
            }

            // стрелки
            else if (c == '\x01' && history_count > 0) {
                if (history_index == -1) history_index = history_count - 1;
            else if (history_index > 0) history_index--;

                for(int i = 0; i < 60; i++) {
                put_char(' ', current_row * 80 + 4 + i, 0x0F);
                }
                    
                char_idx = 0;
                char *cmd = history[history_index % HISTORY_SIZE];

                while (cmd[char_idx] != '\0' && char_idx < 60) {
                input_buffer[char_idx] = cmd[char_idx];
                put_char(cmd[char_idx], current_row * 80 + 4 + char_idx, 0x0F);
                 char_idx++;
                }
                    input_buffer[char_idx] = '\0';
                    update_cursor(current_row, 4 + char_idx);
            }

            else if (c == '\x02' && history_count > 0) {
                if (history_index < history_count - 1) history_index++;

                for(int i = 0; i < 60; i++) {
                put_char(' ', current_row * 80 + 4 + i, 0x0F);
                    }

                    char_idx = 0;
                    char *cmd = history[history_index % HISTORY_SIZE];

                    while (cmd[char_idx] != '\0' && char_idx < 60) {
                    input_buffer[char_idx] = cmd[char_idx];
                    put_char(cmd[char_idx], current_row * 80 + 4 + char_idx, 0x0F);
                    char_idx++;
                    }

                input_buffer[char_idx] = '\0';
                update_cursor(current_row, 4 + char_idx);
            }

            else if (char_idx < 60 && c != '\b' && c != '\n') {
                input_buffer[char_idx] = c;
                put_char(c, current_row * 80 + 4 + char_idx, 0x0F);
                char_idx++;
                update_cursor(current_row, 4 + char_idx);
            }
        }

        asm volatile("hlt");
    }
}
