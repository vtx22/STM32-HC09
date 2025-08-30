#include "HC09.hpp"

uint32_t HC09::get_module_baud_rate()
{
    const uint32_t rates_to_test[] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

    const uint8_t at_msg[2] = {'A', 'T'};

    for (const auto &baud_rate : rates_to_test)
    {
        HAL_UART_DeInit(_huart);
        _huart->Init.BaudRate = baud_rate;
        HAL_UART_Init(_huart);

        HAL_UART_Transmit(_huart, at_msg, 2, HAL_MAX_DELAY);

        uint8_t at_response[2] = {0, 0};
        HAL_UART_Receive(_huart, at_response, 2, 2000);

        if (at_response[0] == 'O' && at_response[1] == 'K')
        {
            return baud_rate;
        }

        HAL_Delay(100);
    }

    return 0;
}

bool HC09::send_command_string(const char *command)
{
    return send_command_string(command, true);
}

bool HC09::send_command_string(const char *command, bool check_response)
{
    HAL_UART_Transmit(_huart, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);

    if (!check_response)
    {
        return false;
    }

    uint8_t response[2] = {0};
    HAL_UART_Receive(_huart, response, 2, HAL_MAX_DELAY);

    return (response[0] == '0' && response[1] == 'K');
}
