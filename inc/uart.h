/**
 * @file uart.h
 * @brief 串口通信模块头文件
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h> // 包含标准库头文件
#include <ioCC2530.h>
#include "uart_config.h"

/**
 * @brief 初始化串口
 * @param baudrate 波特率
 */
void uart_init(uart_baudrate_t baudrate);

/**
 * @brief 发送字节
 * @param data 要发送的字节
 */
void uart_tx_byte(uint8_t data);

/**
 * @brief 发送字符串
 * @param data 要发送的字符串
 * @param length 字符串长度
 */
void uart_tx_str(uint8_t *data, uint16_t length);

/**
 * @brief 发送整数
 * @param data 要发送的整数
 */
void uart_tx_int(int32_t data);

/**
 * @brief 发送浮点数
 * @param data 要发送的浮点数
 */
void uart_tx_float(float data);

/**
 * @brief 设置波特率
 * @param baudrate 波特率
 */
void uart_set_baud(uart_baudrate_t baudrate);

/**
 * @brief 获取最新接收到的数据
 * @return 最新接收到的数据
 */
uint8_t uart_get_latest_data(void);

/**
 * @brief 检查是否有新数据到达
 * @return 1：有新数据；0：无新数据
 */
uint8_t uart_has_new_data(void);

/**
 * @brief 重置新数据标志
 */
void uart_reset_new_data_flag(void);

#endif /* __UART_H__ */