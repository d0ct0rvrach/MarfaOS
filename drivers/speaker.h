#ifndef SPEAKER_H
#define SPEAKER_H
#include "utils/utils.h"

// Основные функции драйвера
void play_sound(uint32_t nFrequence);
void stop_sound();
void beep();
int test_speaker();

#endif