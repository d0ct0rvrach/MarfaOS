#ifndef RAMDISK_H
#define RAMDISK_H

void ramdisk_init(unsigned int mboot_addr);
void ramdisk_dir();
void ramdisk_cat(char *name);

#endif
