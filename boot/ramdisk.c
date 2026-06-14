#include "ramdisk.h"
#include "../drivers/video.h"
#include "../utils/utils.h"

typedef struct {
    unsigned int mod_start; // address where file starts
    unsigned int mod_end;   // where it ends
    unsigned int cmdline;   // string with file name
    unsigned int reserved;  // spare
} multiboot_module_t;

typedef struct {
    unsigned int flags;        // bitfield flags
    unsigned int mem_lower;    // RAM below 1MB
    unsigned int mem_upper;    // RAM above 1MB
    unsigned int boot_device;  // which disk we booted from
    unsigned int cmdline;      // kernel command line args
    unsigned int mods_count;   // number of modules loaded by GRUB
    unsigned int mods_addr;    // address of modules array
} multiboot_info_t;


static multiboot_module_t *modules; // pointer to modules array in RAM
static unsigned int mod_count;      // number of loaded files

void ramdisk_init(unsigned int mboot_addr) {
    multiboot_info_t *mbi = (multiboot_info_t *)mboot_addr; // access struct fields via address
    mod_count = mbi->mods_count;
    modules = (multiboot_module_t *)mbi->mods_addr; // array addresses, pointers to module 0,1 etc
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
    if (mod_count == 0) { // no files
        kprint("No files in ramdisk\n", current_row++, 0, 0x0F);
        return;
    }
    for (unsigned int i = 0; i < mod_count; i++) {
        char *name = basename((char *)modules[i].cmdline);
        kprint(name, current_row++, 0, 0x0F); // each name on new line
        sleep(500);
    }
}

void ramdisk_cat(char *name) {
    for (unsigned int i = 0; i < mod_count; i++) { // iterate files
        char *fname = basename((char *)modules[i].cmdline);
        if (strcmp(fname, name) == 0) { // found it, yay
            char *data = (char *)modules[i].mod_start; // file data
            unsigned int size = modules[i].mod_end - modules[i].mod_start; // size
            for (unsigned int j = 0; j < size; j++) { // byte-by-byte output
                if (data[j] == 0) break; // null byte check in loop
                put_char(data[j], current_row * 80 + current_col, 0x0F);
                current_col++;
                if (current_col >= 80) { // line wrap
                    current_col = 0;
                    current_row++;
                }
            }
            current_row++;
            return;
        }
    }
    kprint("File not found\n", current_row++, 0, 0x0C); // not found
}
