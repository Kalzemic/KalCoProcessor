#include "coprocessor.h"

uint8_t receive_byte(void)
{
    uint8_t byte;
    HAL_SPI_Receive(&hspi1, &byte, 1, HAL_MAX_DELAY);
    return byte;
}

void receive_matrix(int16_t matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE])
{
    for (uint8_t i = 0; i < matrix_size; i++)
    {
        for (uint8_t j = 0; j < matrix_size; j++)
        {
            uint8_t data[2] = {0};
            HAL_SPI_Receive(&hspi1, &data, 2, HAL_MAX_DELAY);
            matrix[i][j] = (data[0] << 8) | data[1];
        }
    }
}

void send_result(int16_t matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE])
{
    for (uint8_t i = 0; i < matrix_size; i++)
    {
        for (uint8_t j = 0; j < matrix_size; j++)
        {
            uint8_t data[2];
            data[0] = (matrix[i][j] >> 8) & 0xFF;
            data[1] = matrix[i][j] & 0xFF;
            HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
        }
    }
}
void matrix_multiply(void)
{
    memset(matrix_c, 0, sizeof(matrix_c));
    for (uint8_t i = 0; i < matrix_size; i++)
    {
        for (uint8_t j = 0; j < matrix_size; j++)
        {
            for (uint8_t k = 0; k < matrix_size; k++)
            {
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
}

void handle_command(uint8_t command)
{
    switch (command)
    {
    case CMD_BEGIN_CONFIG:
        matrix_size = 0;
        break;
    case CMD_SEND_SIZE:
        matrix_size = receive_byte();
        break;
    case CMD_SEND_MATRIX_A:
        receive_matrix(matrix_a);
        break;
    case CMD_SEND_MATRIX_B:
        receive_matrix(matrix_b);
        break;
    case CMD_EXECUTE_MUL:
        matrix_multiply();
        send_result(matrix_c);
        break;
    }
}

void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpiostruct = {0};

    // Initialize pins 5 6 7 (SCK, MISO, MOSI)
    gpiostruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    gpiostruct.Mode = GPIO_MODE_AF_PP;
    gpiostruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpiostruct);

    // Initialize pin 4
    gpiostruct.Pin = GPIO_PIN_4;
    gpiostruct.Mode = GPIO_MODE_INPUT;
    gpiostruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpiostruct);
}

void MX_SPI_Init(void)
{
    __HAL_RCC_SPI1_CLK_ENABLE();

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_SLAVE;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_HARD_INPUT;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    HAL_SPI_Init(&hspi1);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, 0x00000000U);
}

void main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    while (1)
    {
        uint8_t cmd = receive_byte();
        handle_command(cmd);
    }
}