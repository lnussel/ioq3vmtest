VM=none
CC=gcc
ARCH=$(shell uname -m | sed -e s/i.86/i386/)
ifeq ($(ARCH),x86_64)
  VM=vm_x86_64
else
ifeq ($(ARCH),i386)
  VM=vm_x86
  CC+=-m32
else
ifeq ($(ARCH),ppc)
  VM=vm_powerpc
  CC+=-m32
else
ifeq ($(ARCH),ppc64)
  VM=vm_powerpc
  CC+=-m64
else
$(error arch $(ARCH) is currently not supported)
endif
endif
endif
endif
CFLAGS=-g3 -Wall -O0 -fno-strict-aliasing
CPPFLAGS=-DVM_X86_64_STANDALONE -DHAVE_VM_COMPILED -I../code/qcommon/ -DDEBUG_VM
Q3LCC=../build/release-linux-$(ARCH)/tools/q3lcc
Q3ASM=../build/release-linux-$(ARCH)/tools/q3asm
DO_Q3LCC=$(Q3LCC) -Wf-target=bytecode -Wf-g -o $@ -S -DQ3_VM $< || (/bin/rm -f $@; false)
DO_CC=$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<
DO_Q3ASM=$(Q3ASM) -v -o $@ $< syscalls.ASM
OBJ=vm_interpreted.o dummy.o main.o vm.o standalone.o vmstats.o
V=0

ifeq ($(VM),vm_x86)
#XXX
OBJ += ../build/release-linux-i386/ded/ftola.o
else
ifeq ($(VM),vm_x86_64)
OBJ += vm_x86_64_assembler.o
else
ifeq ($(VM),vm_powerpc)
OBJ += vm_powerpc_asm.o
endif
endif
endif

all: t1.qvm t2.qvm t3.qvm t4.qvm t5.qvm t6.qvm strcpy.qvm main

syscalls.ASM:

t2.c: tests.c
%.c: syscalls.h
%.asm: %.c
	$(DO_Q3LCC)

%.qvm: %.asm
	$(DO_Q3ASM)

main.o: ../code/qcommon/$(VM).c
	$(DO_CC) -I. -DDEBUG_VM

standalone.o: tests.c
standalone.o: standalone.c
	$(DO_CC) -I. -DDEBUG_VM

vm.o: ../code/qcommon/vm.c
	$(DO_CC)

vm_x86_64_assembler.o: ../code/qcommon/vm_x86_64_assembler.c
	$(DO_CC)

vm_powerpc_asm.o: ../code/qcommon/vm_powerpc_asm.c
	$(DO_CC)

vm_interpreted.o: ../code/qcommon/vm_interpreted.c
	$(DO_CC)

dummy.o: dummy.c
	$(DO_CC)

main: $(OBJ)
	$(CC) -g -o $@ $(OBJ)

%.s: main
%.s: %.qvm
	./main $< $@

%.obj: %.s
	as -o $@ $<

%.disasm: %.obj
	objdump -d $<

%.raw: %.obj
	objcopy -O binary -j .text $< $@

c: t1.s

cs: main t2.qvm
	./main t2.qvm

asm: t1.obj

disasm: t1.disasm

raw: t1.raw

clean:
	rm -f *.o *.asm *.qvm *.map *.obj *.s *.disasm *.qdasm *.raw

.SUFFIXES: .asm
.PRECIOUS: %.asm
