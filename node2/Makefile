LDSCRIPT = sam/flash.ld
BOOTUP = sam/sam3x/source/exceptions.c sam/sam3x/source/startup_sam3x.c sam/sam3x/source/system_sam3x.c
MCUTYPE = __SAM3X8E__

# List all source files to be compiled; separate with space
INCLUDE_DIR := include
SOURCE_FILES := $(wildcard $(INCLUDE_DIR)/*.c) src/main.c

# Feel free to ignore anything below this line
VPATH:= $(dir $(SOURCE_FILES))
BUILD_DIR := build
OBJS = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)
#Tools
CC:=arm-none-eabi-gcc
LD:=arm-none-eabi-gcc
AR:=arm-none-eabi-ar
AS:=arm-none-eabi-as

ELF=$(BUILD_DIR)/main.elf

LDFLAGS:= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -Wl,--gc-sections
CFLAGS:= -mcpu=cortex-m3 -mthumb -g -std=c11 -MMD
CFLAGS+= -I sam -I sam/sam3x/include -I sam/sam3x/source -I sam/cmsis -I .
CFLAGS+= -I$(INCLUDE_DIR)
CFLAGS+= -D $(MCUTYPE)
CFLAGS+= -Wall

.DEFAULT_GOAL := $(ELF)
# compile and generate dependancy info
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(INCLUDE_DIR)
	mkdir -p $(BUILD_DIR)/src

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o : %.s | $(BUILD_DIR)
	$(AS) $< -o $@

$(ELF) : $(OBJS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: debug
debug: $(ELF)
	if pgr openocd; then killall -s 9 openocd; fi
	openocd -f sam/openocd.cfg -c "program $(ELF) verify" &
	sleep 5
	arm-eabi-gdb -tui -iex "target extended-remote localhost:3333" $(ELF)
	killall -s 9 openocd	

.PHONY: flash
flash: $(ELF)
	if pgrep openocd; then killall -s 9 openocd; fi
	openocd -f sam/openocd.cfg -c "program ${BUILD_DIR}/main.elf verify reset exit"


# pull in dependencies
-include	$(OBJS:.o=.d)

