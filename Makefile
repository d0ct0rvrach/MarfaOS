CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-builtin -nostdlib -O2 -I.
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

# исходники
C_SOURCES = $(wildcard *.c kernel/*.c drivers/*.c cpu/*.c shell/*.c utils/*.c apps/*.c boot/*.c memory/*.c fsod/*.c )
ASM_SOURCES = $(wildcard cpu/*.asm boot/*.asm)

# объекты
ALL_OBJ = ${C_SOURCES:.c=.o} ${ASM_SOURCES:.asm=.o}

# boot.o всегда первый — линкер требует
OBJ = boot/boot.o $(filter-out boot/boot.o, $(ALL_OBJ))

all: marfa_os.bin

marfa_os.bin: ${OBJ}
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.bin *.o drivers/*.o shell/*.o utils/*.o cpu/*.o apps/*.o kernel/*.o boot/*.o memory/*.o

run: all
	qemu-system-i386 -kernel marfa_os.bin -audiodev sdl,id=audio0 -machine pcspk-audiodev=audio0
