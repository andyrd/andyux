BLASM    = bootloader.asm
BLBIN    = bootloader.bin
IMAGE    = andyux_v1.flp
AS       = nasm
ASFLAGS  = -f bin -o
DD       = dd
DDFLAGS  = status=noxfer conv=notrunc if=$(BLBIN) of=$(IMAGE)
EMU      = qemu
EMUFLAGS = -fda

all: $(IMAGE)

$(IMAGE): $(BLBIN)
	$(DD) $(DDFLAGS) 

$(BLBIN): $(BLASM)
	$(AS) $(ASFLAGS) $(BLBIN) $(BLASM)

test: $(IMAGE)
	$(EMU) $(EMUFLAGS) $(IMAGE)

clean:
	rm -rf *bin