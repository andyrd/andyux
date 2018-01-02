OSIMG     = andyux.img
OSBIN     = andyux.bin
KERNELDIR = kernel
KERNELSRC = $(wildcard $(KERNELDIR)/*.c)
KERNELOBJ = $(patsubst %.c,%.o,$(KERNELSRC))
KERASMSRC = $(wildcard $(KERNELDIR)/*.s)
KERASMOBJ = $(patsubst %.s,%.o,$(KERASMSRC))
LOADERDIR = loader
LOADERSRC = $(wildcard $(LOADERDIR)/*.s)
LOADEROBJ = $(patsubst %.s,%.o,$(LOADERSRC))
LINKSRC   = link.ld
AS        = nasm
ASFLAGS   = -f elf
CC        = gcc 
#CFLAGS    = -nostdlib -nostdinc -fno-builtin -fno-stack-protector
CFLAGS    = -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -std=gnu99 -m32
LD        = ld
LDFLAGS   = -melf_i386
EMU       = qemu-system-i386 -kernel

all: $(OSIMG)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

$(OSBIN): $(KERNELOBJ) $(KERASMOBJ) $(LOADEROBJ)
	$(LD) $(LDFLAGS) -T $(LINKSRC) -o $@ $^

$(OSIMG): $(OSBIN)
	dd if=/dev/zero of=pad bs=1 count=750
	cat grub/stage1 grub/stage2 pad $< > $@

test: $(OSBIN)
	$(EMU) $(OSBIN)

clean:
	rm -f $(KERNELDIR)/*.o $(LOADERDIR)/*.o *.bin *.img pad