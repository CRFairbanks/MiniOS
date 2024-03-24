BOOT = minios.iso
KERNEL = kernel_x86_64
USER = user_x86_64

CC = gcc
LD = ld
CFLAGS += -Wall -O2 -mno-red-zone -nostdinc -fno-stack-protector -pie -fno-zero-initialized-in-bss -c
LDFLAGS = --oformat=binary -T ./kernel/kernel.lds -nostdlib -melf_x86_64 -pie
KERNEL_OBJS = kernel/kernel_entry.o # Do not reorder this one
KERNEL_OBJS += kernel/kernel.o kernel/kernel_asm.o kernel/string.o kernel/fb.o kernel/printf.o kernel/ascii_font.o kernel/kernel_code.o kernel/kernel_malloc.o kernel/kernel_extra.o
USER_OBJS = user/user_entry.o # Do not reoder this one
USER_OBJS += user/user.o

all: $(BOOT)

$(BOOT): $(KERNEL) $(USER) boot.efi
	@rm -rf uefi_iso_image
	@mkdir -p uefi_iso_image/EFI/BOOT
	@cp boot.efi uefi_iso_image/EFI/BOOT/BOOTX64.EFI
	@cp $(KERNEL) uefi_iso_image/EFI/BOOT/KERNEL
	@cp $(USER) uefi_iso_image/EFI/BOOT/USER
	@mkisofs -input-charset=ascii -o $(BOOT) uefi_iso_image

$(KERNEL): $(KERNEL_OBJS) kernel/page_load.o
	$(LD) $(LDFLAGS) $^ -o $@

$(USER): $(USER_OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -I ./kernel/include -c -o $@ $<

kernel/%.o: kernel/%.S
	$(CC) $(CFLAGS) -I ./kernel/include -c -o $@ $<

user/%.o: user/%.c
	$(CC) $(CFLAGS) -I ./user/include -c -o $@ $<

clean:
	@rm -rf $(KERNEL) $(USER) $(KERNEL_OBJS) $(USER_OBJS) $(BOOT) uefi_iso_image
