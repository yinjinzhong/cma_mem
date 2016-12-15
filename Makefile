
ccflags-y  := -DDEBUG
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)

cma-objs	:= cma_mem.o
obj-m		:= cma.o
#helloworld-objs := file1.o file2.o

# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else

.PHONY: all clean

KERNELDIR := ../linux-atlas7-make-mr1/kernel-source/build-kernel-out
PWD       := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- M=$(PWD) modules
	rm -rf *.o *.mod.c .*.cmd .*tmp_versions *.symvers *.order

##	./scp_to_remote.sh

clean:
	rm -rf *.o *.mod.c .*.cmd .*tmp_versions *.symvers *.order *.ko

# End of KERNELRELEASE.
endif
