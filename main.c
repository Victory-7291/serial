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
    
    // 发送启动消息
    uart_tx_str("\r\nUART Demo Started!\r\n", 21);
    uart_tx_str("Baudrate: 115200\r\n", 18);

    while (1)
    {
        // 发送字符串
        uart_tx_str("Hello, UART!\r\n", 14);

        // 发送整数
        int32_t num = 12345;
        uart_tx_str("Int: ", 5);
        uart_tx_int(num);
        uart_tx_str("\r\n", 2);

        // 发送浮点数
        float f = 3.14159;
        uart_tx_str("Float: ", 7);
        uart_tx_float(f);
        uart_tx_str("\r\n", 2);

        // 等待1秒
        delay(1000);

        // 检查是否有数据到来
        if (uart_has_new_data())
        {
            // 获取接收数据
            uint8_t data = uart_get_latest_data();
            uart_reset_new_data_flag(); // 清除接收数据标志
            
            uart_tx_str("Received: ", 10);
            uart_tx_byte(data);
            uart_tx_str("\r\n", 2);

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
    // 简单的数据处理逻辑
    uart_tx_byte(data); // 回传数据
}