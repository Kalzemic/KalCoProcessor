#ifndef _COPROCESSOR_
#define _COPROCESSOR_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_dma.h"
#include <stdint.h>
#include "protocol.h"
#include "stm32f1xx_hal_def.h"
#include <string.h>
#include "stm32f1xx_hal_rcc.h"
#include "core_cm3.h"
extern SPI_HandleTypeDef hspi1;
uint8_t matrix_size = 0;
int16_t matrix_a[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
int16_t matrix_b[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
int16_t matrix_c[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

uint8_t receive_byte(void);
void receive_matrix(int16_t matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]);
void send_result(int16_t matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]);
void matrix_multiply(void);
void handle_command(uint8_t command);
void MX_GPIO_Init(void);
void MX_SPI1_Init(void);
void SystemClock_Config(void);

#endif //_COPROCESSOR_