WORKSPACE = .
CC = gcc
CFLAGS += -std=c99 -Wall -DHAS_LOG=6
CFLAGS += -I$(WORKSPACE)/generated -I$(WORKSPACE)/include
LDFLAGS = -shared

TARGET = libai_network
BUILD_DIR = ./$(WORKSPACE)/build
LIBS = -L$(WORKSPACE)/lib/static -lruntime -lst_cmsis_nn -lcmsis_nn -lx86_cmsis -lm

# OS-specific shared library build
ifeq ($(OS),Windows_NT)
	TARGET := $(TARGET).dll
	LDFLAGS += -Wl,--out-implib,$(BUILD_DIR)/$(TARGET).a
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		TARGET := $(TARGET).dylib
	else
		TARGET := $(TARGET).so
	endif
	CFLAGS += -fPIC
endif


SOURCES = $(wildcard $(WORKSPACE)/generated/*.c)
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJECTS:%.o=%.d)

copy_generated_files:
	mkdir -p $(WORKSPACE)/generated
	cp -f generated/network* $(WORKSPACE)/generated

.PHONY: install default all clean copy_generated_files

default: $(BUILD_DIR)/$(TARGET)
all: default

install: all
	mv $(BUILD_DIR)/$(TARGET) "$(WORKSPACE)/lib"

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

-include $(DEP)

$(BUILD_DIR)/%.o: %.c $(HEADERS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	-rm -f $(BUILD_DIR)/$(TARGET) $(OBJECTS) $(DEP)
	-rm -f $(TARGET)
	-rm -rf $(BUILD_DIR)
