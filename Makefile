#ToolChain
CC= arm-none-eabi-gcc
AS= arm-none-eabi-as 
LD= arm-none-eabi-ld 
OBJCOPY= arm-none-eabi-objcopy 
SIZE= arm-none-eabi-size 

CPU= -mcpu=cortex-m3 -mthumb


#Paths

CMSIS_DIR = ./Drivers/CMSIS
HAL_DIR = ./Drivers/STM32F1xx_HAL_Driver



INCLUDES = -I. \
           -I$(CMSIS_DIR)/Include \
           -I$(CMSIS_DIR)/Device/ST/STM32F1xx/Include \
           -I$(HAL_DIR)/Inc \
		   -I./core/Inc


CFLAGS = $(CPU) -Wall -O2 -g -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB $(INCLUDES)
ASFLAGS = $(CPU)
LDFLAGS = $(CPU) -Tlinker.ld -nostartfiles -nostdlib -Wl,--gc-sections

# Source files
SRCS = startup.s \
       coprocessor.c \
       $(HAL_DIR)/Src/stm32f1xx_hal.c \
       $(HAL_DIR)/Src/stm32f1xx_hal_gpio.c \
       $(HAL_DIR)/Src/stm32f1xx_hal_spi.c \
       $(HAL_DIR)/Src/stm32f1xx_hal_rcc.c \
       $(HAL_DIR)/Src/stm32f1xx_hal_cortex.c




OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

TARGET = coprocessor
ELF = $(TARGET).elf
BIN = $(TARGET).bin

all:$(BIN)



$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<


$(ELF): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s 
	arm-none-eabi-as  $(ASFLAGS) $< -o $@

clean:
	rm -f *.o *.elf *.bin

.PHONY: all clean