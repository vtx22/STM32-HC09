#include "HC09.hpp"

uint32_t HC09::get_module_baud_rate()
{
    const uint32_t rates_to_test[] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

    const char *at_msg = "AT";
    char at_response[2] = {0};

    for (const auto &baud_rate : rates_to_test)
    {
        HAL_UART_DeInit(_huart);
        _huart->Init.BaudRate = baud_rate;
        HAL_UART_Init(_huart);

        HAL_UART_Transmit(_huart, (uint8_t *)at_msg, 2, 100);

        char at_response[2] = {0};
        HAL_UART_Receive(_huart, (uint8_t *)at_response, 2, 100);

        if (at_response[0] == 'O' && at_response[1] == 'K')
        {
            return baud_rate;
        }
    }

    return 0;
}

void HC09::send_command_string(const char *command)
{
    HAL_UART_Transmit(_huart, (uint8_t *)command, strlen(command), 100);
}