/**
 * @file uart_config.h
 * @brief 串口配置头文件
 */

#ifndef __UART_CONFIG_H__
#define __UART_CONFIG_H__

/**
 * @brief 波特率枚举
 */
typedef enum {
    UART_BAUD_9600     = 9600,
    UART_BAUD_19200   = 19200,
    UART_BAUD_38400   = 38400,
    UART_BAUD_57600   = 57600,
    UART_BAUD_115200 = 115200
} uart_baudrate_t;

/**
 * @brief 默认波特率
 */
#define UART_DEFAULT_BAUD UART_BAUD_115200

/**
 * @brief 接收缓冲区大小
 */
#define UART_RX_BUF_SIZE  128

/**
 * @brief 发送缓冲区大小
 */
#define UART_TX_BUF_SIZE  128

#endif /* __UART_CONFIG_H__ */