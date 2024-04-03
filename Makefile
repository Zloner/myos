GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore  #编译时需要取消g++自动添加的链接库
ASMPARAMS = --32
LDPARAMS = -melf_i386

#编译程序#
 objects=kernel.o loader.o gdt.o port.o interrupts.o interruptstubs.o

 %.o: %.cpp  #  %用来替换
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.s      #汇编
	as $(ASMPARAMS) -o $@ $<

mykernel.bin:linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

#能够启动的光盘镜像文件#
mykernel.iso:mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout = 5' >> iso/boot/grub/grub.cfg
	echo 'set default = 0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "my operating system" {' >> iso/boot/grub/grub.cfg
	echo ' multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo ' boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run:mykernel.iso


.phony:clean 
clean:
	rm -rf $(objects) mykernel.bin



