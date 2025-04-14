/**
 * @file uart.c
 * @brief 串口通信模块实现文件
 */

#include "uart.h"
#include <string.h>
#include <stdint.h> // 标准头文件

/**
 * @brief 最新接收的数据
 */
static volatile uint8_t latest_data = 0;
/**
 * @brief 数据接收标志
 */
static volatile uint8_t new_data_flag = 0;

/**
 * @brief 初始化串口
 * @param baudrate 波特率
 */
void uart_init(uart_baudrate_t baudrate)
{
    // 设置系统时钟源为32MHz
    CLKCONCMD &= ~0x40;
    while (CLKCONSTA & 0x40);
    CLKCONCMD &= ~0x47;

    // 设置P0的P0_2和P0_3为UART0引脚
    PERCFG = 0x00;       // 将UART0设为可选位置1（P0.2-3）
    P0SEL |= 0x0C;       // 设置P0.2和P0.3为外设功能
    P2DIR &= ~0xC0;      // P0优先级最高

    // 设置UART模式
    U0CSR |= 0x80;       // UART模式使能（非SPI）

    // 设置波特率
    uart_set_baud(baudrate);

    // 初始化发送和接收中断标志
    UTX0IF = 0;          // 清除TX中断标志
    URX0IF = 0;          // 清除RX中断标志

    // 启用接收
    U0CSR |= 0x40;       // 接收器使能

    // 启用中断
    IEN0 |= 0x04;        // 设置UART0 RX中断使能(URX0IE)
    EA = 1;              // 全局中断使能
}

/**
 * @brief 发送字节
 * @param data 要发送的字节
 */
void uart_tx_byte(uint8_t data)
{
    // 等待发送完成
    while (UTX0IF == 0);
    UTX0IF = 0;

    // 发送数据
    U0DBUF = data;
}

/**
 * @brief 发送字符串
 * @param data 要发送的字符串
 * @param length 字符串长度
 */
void uart_tx_str(uint8_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        uart_tx_byte(data[i]);
    }
}

/**
 * @brief 发送整数
 * @param data 要发送的整数
 */
void uart_tx_int(int32_t data)
{
    uint8_t buffer[4];
    memcpy(buffer, &data, 4);
    uart_tx_str((uint8_t *)buffer, 4);
}

/**
 * @brief 发送浮点数
 * @param data 要发送的浮点数
 */
void uart_tx_float(float data)
{
    uint8_t buffer[4];
    memcpy(buffer, &data, 4);
    uart_tx_str((uint8_t *)buffer, 4);
}

/**
 * @brief 设置波特率
 * @param baudrate 波特率
 */
void uart_set_baud(uart_baudrate_t baudrate)
{
    // 根据不同的波特率设置U0GCR和U0BAUD
    switch (baudrate)
    {
        case UART_BAUD_9600:
            U0GCR &= ~0x1F;
            U0GCR |= 8;
            U0BAUD = 59;
            break;
        case UART_BAUD_19200:
            U0GCR &= ~0x1F;
            U0GCR |= 9;
            U0BAUD = 59;
            break;
        case UART_BAUD_38400:
            U0GCR &= ~0x1F;
            U0GCR |= 10;
            U0BAUD = 59;
            break;
        case UART_BAUD_57600:
            U0GCR &= ~0x1F;
            U0GCR |= 10;
            U0BAUD = 216;
            break;
        case UART_BAUD_115200:
        default:
            U0GCR &= ~0x1F;
            U0GCR |= 11;
            U0BAUD = 216;
            break;
    }
}

/**
 * @brief 接收中断服务函数
 */
#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{
    URX0IF = 0; // 清除接收中断标志
    uint8_t data = U0DBUF;

    // 存储接收到的数据
    latest_data = data;
    new_data_flag = 1;
}

/**
 * @brief 获取最新接收的数据
 * @return 最新接收的数据
 */
uint8_t uart_get_latest_data(void)
{
    return latest_data;
}

/**
 * @brief 是否有新数据
 * @return 1有，0没有
 */
uint8_t uart_has_new_data(void)
{
    return new_data_flag;
}

/**
 * @brief 清除接收标志
 */
void uart_reset_new_data_flag(void)
{
    new_data_flag = 0;
}

// 文件结束