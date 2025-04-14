/**
 * @file uart.c
 * @brief 串口通信模块实现文件
 */

#include "uart.h"
#include <string.h>
#include <stdint.h> // 包含标准库头文件

/**
 * @brief 最新接收到的数据
 */
static volatile uint8_t latest_data = 0;
/**
 * @brief 新数据标志
 */
static volatile uint8_t new_data_flag = 0;

/**
 * @brief 初始化串口
 * @param baudrate 波特率
 */
void uart_init(uart_baudrate_t baudrate)
{
    // 设置系统时钟源为 32MHz 晶振
    CLKCONCMD &= ~0x40;
    while (CLKCONSTA & 0x40);
    CLKCONCMD &= ~0x47;

    // 设置 P0 口的 P0_2 和 P0_3 为 UART0 功能
    PERCFG = 0x00;
    P0SEL = 0x0C;

    // 设置为 UART 模式
    U0CSR |= 0x80;

    // 设置波特率
    uart_set_baud(baudrate);

    // 初始化发送和接收中断标志
    UTX0IF = 0;
    URX0IF = 0;

    // 允许接收
    U0CSR |= 0x40;

    // 开启接收中断
    IEN0 |= 0x84;
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
    uart_tx_str(buffer, 4);
}

/**
 * @brief 发送浮点数
 * @param data 要发送的浮点数
 */
void uart_tx_float(float data)
{
    uint8_t buffer[4];
    memcpy(buffer, &data, 4);
    uart_tx_str(buffer, 4);
}

/**
 * @brief 设置波特率
 * @param baudrate 波特率
 */
void uart_set_baud(uart_baudrate_t baudrate)
{
    // 根据波特率计算 U0GCR 和 U0BAUD 的值
    uint32_t baud_div = (32000000 / (baudrate * 16)) - 1;
    U0GCR = (baud_div >> 8) & 0xFF;
    U0BAUD = baud_div & 0xFF;
}

/**
 * @brief 串口接收中断服务程序
 */
#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{
    URX0IF = 0; // 清中断标志
    uint8_t data = U0DBUF;

    // 更新最新数据和标志
    latest_data = data;
    new_data_flag = 1;
}

/**
 * @brief 获取最新接收到的数据
 * @return 最新接收到的数据
 */
uint8_t uart_get_latest_data(void)
{
    return latest_data;
}

/**
 * @brief 检查是否有新数据到达
 * @return 1：有新数据；0：无新数据
 */
uint8_t uart_has_new_data(void)
{
    return new_data_flag;
}

/**
 * @brief 重置新数据标志
 */
void uart_reset_new_data_flag(void)
{
    new_data_flag = 0;
}