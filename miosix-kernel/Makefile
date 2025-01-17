##
## Makefile for Miosix embedded OS
##
MAKEFILE_VERSION := 1.07

## Path to kernel directory (edited by init_project_out_of_git_repo.pl)
KPATH := miosix

## Path to config directory (edited by init_project_out_of_git_repo.pl)
CONFPATH := $(KPATH)

## MODULE PATHS
AIPATH := ./AI
SENSDRIV := sensor_driver
DATAUTL := data_util

ARM_MIOSIX_EABI := C:/arm-miosix-eabi
ARM_NONE_EABI := C:/Program Files (x86)/GNU Tools ARM Embedded/4.8 2014q3
PROJ_DIR := C:\Users\grgcz\Desktop\dev_space_win\AOS_Project\AOS_MovementRecognition\miosix-kernel
include $(CONFPATH)/config/Makefile.inc
PROGRAM_CMDLINE := Copy-Item .\main.bin F:\
##
## List here subdirectories which contains makefiles
##
SUBDIRS := $(KPATH)			\
		
##
## List here your source files (both .s, .c and .cpp)
##
SRC :=                                							\
./main.cpp														\
$(SENSDRIV)/com_i2c/i2c_helper.cpp								\
$(DATAUTL)/data_proc.cpp										\
$(SENSDRIV)/LSM6DSL/LSM6DSL.cpp									\
$(SENSDRIV)/LSM303AGR/LSM303AGR.cpp								\
$(AIPATH)/Src/NN.cpp											\
$(AIPATH)/eabi_glibc/aeabi_memclr.c								\
$(AIPATH)/eabi_glibc/aeabi_memcpy.c 							\
$(AIPATH)/Src/network_data.c 									\
$(AIPATH)/Src/network.c  										\



##
## List here additional static libraries with relative path
##
LIBS :=	\
$(AIPATH)/Middlewares/ST/AI/Lib/NetworkRuntime500_CM4_GCC.a               \

##
## List here additional include directories (in the form -Iinclude_dir)
##
INCLUDE_DIRS :=			\
-I$(SENSDRIV)/									\
-I$(SENSDRIV)/board_common						\
-I$(SENSDRIV)/LSM6DSL							\
-I$(SENSDRIV)/LSM303AGR							\
-I$(SENSDRIV)/com_i2c							\
-I$(DATAUTL)									\
-I$(AIPATH)										\
-I$(AIPATH)/Inc 								\
-I$(AIPATH)/Middlewares/ST/AI/Inc				\

##############################################################################
## You should not need to modify anything below                             ##
##############################################################################


# macros for gcc
# AS defines
AS_DEFS = 

# ?? Unexpected required flags to compile ??
# C defines
C_DEFS =  \
-DARM_MATH_CM4 \
-D__FPU_PRESENT=1U
###############################################################################

ifeq ("$(VERBOSE)","1")
Q := 
ECHO := @true
else
Q := @
ECHO := @echo
endif

## Replaces both "foo.cpp"-->"foo.o" and "foo.c"-->"foo.o"
OBJ := $(addsuffix .o, $(basename $(SRC)))

## Includes the miosix base directory for C/C++
## Always include CONFPATH first, as it overrides the config file location
CXXFLAGS := $(CXXFLAGS_BASE) -I$(CONFPATH) -I$(CONFPATH)/config/$(BOARD_INC)  					\
            -I. -I$(KPATH) -I$(KPATH)/arch/common -I$(KPATH)/$(ARCH_INC)      					\
            -I$(KPATH)/$(BOARD_INC) $(INCLUDE_DIRS)							  					\

CFLAGS   := $(CFLAGS_BASE) $(C_DEFS) -std=c99 -I$(CONFPATH) -I$(CONFPATH)/config/$(BOARD_INC)  \
            -I. -I$(KPATH) -I$(KPATH)/arch/common -I$(KPATH)/$(ARCH_INC)      \
            -I$(KPATH)/$(BOARD_INC) $(INCLUDE_DIRS)							\
			
AFLAGS   := $(AFLAGS_BASE)
LFLAGS   := $(LFLAGS_BASE)
DFLAGS   := -MMD -MP

LINK_LIBS :=  $(LIBS) -L$(PROJ_DIR)/$(KPATH) -Wl,--start-group -lmiosix $(ARCH_LIBS) -Wl,--end-group

all: all-recursive main

clean: clean-recursive clean-topdir

program:
	$(PROGRAM_CMDLINE)

all-recursive:
	$(foreach i,$(SUBDIRS),$(MAKE) -C $(i)                               \
	  KPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(KPATH))       \
	  CONFPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(CONFPATH)) \
	  || exit 1;)

clean-recursive:
	$(foreach i,$(SUBDIRS),$(MAKE) -C $(i)                               \
	  KPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(KPATH))       \
	  CONFPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(CONFPATH)) \
	  clean || exit 1;)

clean-topdir:
	-rm -f $(OBJ) main.elf main.hex main.bin main.map $(OBJ:.o=.d)

main: main.elf
	$(ECHO) "[CP  ] main.hex"
	$(Q)$(CP) -O ihex   main.elf main.hex
	$(ECHO) "[CP  ] main.bin"
	$(Q)$(CP) -O binary main.elf main.bin
	$(Q)$(SZ) main.elf

main.elf: $(OBJ) all-recursive
	$(ECHO) "[LD  ] main.elf"
	$(Q)$(ARM_NONE_EABI)/bin/arm-none-eabi-gcc $(LFLAGS) -o main.elf $(OBJ) $(KPATH)/$(BOOT_FILE) $(LINK_LIBS)


%.o: %.s
	$(ECHO) "[AS  ] $<"
	$(Q)$(AS)  $(AFLAGS) $< -o $@

%.o : %.c
	$(ECHO) "[CC  ] $<"
	$(Q)$(CC)  $(DFLAGS) $(CFLAGS) $< -o $@
	
%.o : %.cpp
	$(ECHO) "[CXX ] $<"
	$(Q)$(CXX) $(DFLAGS) $(CXXFLAGS) $< -o $@


#pull in dependecy info for existing .o files
-include $(OBJ:.o=.d)
