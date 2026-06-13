#include "ramdisk.h"
#include "../drivers/video.h"
#include "../utils/utils.h"

typedef struct {
    unsigned int mod_start; //адрес где начинается файл
    unsigned int mod_end; // заканчивается
    unsigned int cmdline; // строка с именем ф
    unsigned int reserved; // запаска
} multiboot_module_t;

typedef struct {
    unsigned int flags; // битовые флаги 
    unsigned int mem_lower;  // сколько озу ниже 1мб
    unsigned int mem_upper; // сколько озу высше 1мб
    unsigned int boot_device; // с какого диска загрузилсь
    unsigned int cmdline; // аргументы кс ядра
    unsigned int mods_count; // кол модулей загрузки грабом
    unsigned int mods_addr; // адрес ма-ва
} multiboot_info_t;


static multiboot_module_t *modules; // указатекль на ма-ва модулей в озу
static unsigned int mod_count; //кол заг-ых файлов

void ramdisk_init(unsigned int mboot_addr) {
    multiboot_info_t *mbi = (multiboot_info_t *)mboot_addr; // обращение к полям + адресс структуры
    mod_count = mbi->mods_count;
    modules = (multiboot_module_t *)mbi->mods_addr; // короче адреса масивов тут указатели там модуль 0,1 итд
}

static char *basename(char *path) {
    char *last = path;
    while (*path) {
        if (*path == '/') last = path + 1;
        path++;
    }
    return last;
}

void ramdisk_dir() {
    if (mod_count == 0) { //Елсли нет файлов
        kprint("No files in ramdisk\n", current_row++, 0, 0x0F);
        return;
    }
    for (unsigned int i = 0; i < mod_count; i++) {
        char *name = basename((char *)modules[i].cmdline);
        kprint(name, current_row++, 0, 0x0F); //каждое имя в новой строке
        sleep(500);
    }
}

void ramdisk_cat(char *name) {
    for (unsigned int i = 0; i < mod_count; i++) { // перебор файлов
        char *fname = basename((char *)modules[i].cmdline);
        if (strcmp(fname, name) == 0) { // нашли нужный ура
            char *data = (char *)modules[i].mod_start; // данные файла
            unsigned int size = modules[i].mod_end - modules[i].mod_start; // размер
            for (unsigned int j = 0; j < size; j++) { // побайтовый вывод
                if (data[j] == 0) break; // проверка на нулевой байт в цикле
                put_char(data[j], current_row * 80 + current_col, 0x0F);
                current_col++;
                if (current_col >= 80) { // перенос строки
                    current_col = 0;
                    current_row++;
                }
            }
            current_row++;
            return;
        }
    }
    kprint("File not found\n", current_row++, 0, 0x0C); // не нашли крч
}
