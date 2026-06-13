#ifndef KEYBOARD_H
#define KEYBOARD_H

void init_keyboard();

char scan_to_char(unsigned char scancode);
int test_ps2();
void check_cpu();
void keyboard_callback();
char get_char();

#endif