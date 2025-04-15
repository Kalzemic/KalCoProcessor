INCLUDES = \
-I./Core/Inc \
-I./Drivers/CMSIS/Include \
-I./Drivers/CMSIS/Device/ST/STM32F1xx/Include \
-I./Drivers/STM32F1xx_HAL_Driver/Inc


SRCS += \
Core/Src/main.c \
Core/Src/stm32f1xx_hal_msp.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
