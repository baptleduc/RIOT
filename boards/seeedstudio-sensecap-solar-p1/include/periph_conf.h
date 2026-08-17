/*
 * SPDX-FileCopyrightText: 2026 Léandre Le Duc <leandre.leduc@etik.com>
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#pragma once

/**
 * @ingroup     boards_seeedstudio-sensecap-solar-p1
 * @{
 *
 * @file
 * @brief       Peripheral configuration for the SeeedStudio SenseCAP Solar Node P1
 *
 * @author      Léandre Le Duc <leandre.leduc@etik.com>
 */

#include "periph_cpu.h"
#include "cfg_clock_32_1.h"
#include "cfg_rtt_default.h"
#include "cfg_timer_default.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    UART configuration
 *
 * UART_DEV(0): GNSS module (L76K), 9600 baud NMEA
 * @{
 */
static const uart_conf_t uart_config[] = {
    {
        .dev = NRF_UARTE0,
        .rx_pin = GPIO_PIN(1, 12),          /* SOLAR_GPS_RX */
        .tx_pin = GPIO_PIN(1, 11),          /* SOLAR_GPS_TX */
#ifdef MODULE_PERIPH_UART_HW_FC
        .rts_pin = GPIO_UNDEF,
        .cts_pin = GPIO_UNDEF,
#endif
        .irqn = UARTE0_UART0_IRQn,
    },
};

#define UART_0_ISR          (isr_uart0)

#define UART_NUMOF          ARRAY_SIZE(uart_config)
/** @} */

/**
 * @name    SPI configuration
 *
 * SPI_DEV(0): SX1262 LoRa transceiver
 * @{
 */
static const spi_conf_t spi_config[] = {
    {
        .dev = NRF_SPIM2,
        .sclk = GPIO_PIN(1, 13),    /* SOLAR_LORA_CLK_PIN */
        .mosi = GPIO_PIN(1, 15),    /* SOLAR_LORA_MOSI_PIN */
        .miso = GPIO_PIN(1, 14),    /* SOLAR_LORA_MISO_PIN */
    },
};

#define SPI_NUMOF           ARRAY_SIZE(spi_config)
/** @} */

/**
 * @name    I2C configuration
 *
 * I2C_DEV(0): Grove connector, nothing is populated on the board itself
 *
 * @note    These pins are the NFC antenna pins, see the board documentation
 * @{
 */
static const i2c_conf_t i2c_config[] = {
    {
        .dev = NRF_TWIM0,
        .scl = GPIO_PIN(0, 10),     /* SOLAR_GROVE_D0 */
        .sda = GPIO_PIN(0, 9),      /* SOLAR_GROVE_D1 */
        .speed = I2C_SPEED_NORMAL,
    },
};

#define I2C_NUMOF           ARRAY_SIZE(i2c_config)
/** @} */

/**
 * @name    ADC configuration
 *
 * ADC_NUMOF is fixed by the nRF52 CPU (9 or 10 channels).
 * The only line used on this board is NRF52_AIN7 (P0.31), the battery
 * voltage divider.
 * @{
 */
/* ADC_NUMOF defined in cpu/nrf52/include/periph_cpu.h */
/** @} */

#ifdef __cplusplus
}
#endif

/** @} */
