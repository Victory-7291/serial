/**
 * @file main.c
 * @brief 串口通信模块演示程序
 */

#include <ioCC2530.h>
#include "uart.h"

// 声明 process_data 函数
void process_data(uint8_t data);

void delay(uint32_t ms)
{
    uint32_t i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 3000; j++);
    }
}

int main(void)
{
    // 初始化串口，设置波特率为 115200
    uart_init(UART_BAUD_115200);

    while (1)
    {
        // 发送字符串
        uart_tx_str("Hello, UART!\r\n", 13);

        // 发送整数
        int32_t num = 12345;
        uart_tx_int(num);

        // 发送浮点数
        float f = 3.14159;
        uart_tx_float(f);

        // 延时 1 秒
        delay(1000);

        // 检查是否有新数据到达
        if (uart_has_new_data())
        {
            // 获取最新数据
            uint8_t data = uart_get_latest_data();
            uart_reset_new_data_flag(); // 重置新数据标志

            // 处理数据
            process_data(data);
        }
    }
}

/**
 * @brief 处理接收到的数据
 * @param data 接收到的数据
 */
void process_data(uint8_t data)
{
    // 在这里实现数据处理逻辑
    uart_tx_byte(data); // 回传数据
}