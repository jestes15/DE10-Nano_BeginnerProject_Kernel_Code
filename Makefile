KDIR ?= ../linux-socfpga

SRC_DIR = src
OBJ_DIR = obj
MOD_FILES := $(wildcard $(SRC_DIR)/*.ko)

default:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR) clean

help:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR) help
