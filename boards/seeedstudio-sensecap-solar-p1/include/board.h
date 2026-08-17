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
 * @brief       Board specific definitions for the SeeedStudio SenseCAP Solar Node P1
 *
 * Pin assignments are taken from the Meshtastic board variant maintained by
 * SeeedStudio, see
 * https://github.com/meshtastic/firmware/tree/develop/variants/nrf52840/seeed_solar_node
 *
 * @author      Léandre Le Duc <leandre.leduc@etik.com>
 */

#include "kernel_defines.h"
#include "periph/gpio.h"
#if IS_USED(MODULE_SX126X_RF_SWITCH)
#  include "sx126x.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Button pin definitions
 *
 * Both buttons pull their line to ground when pressed, the internal pull-up
 * has to be enabled.
 * @{
 */
#define BTN0_PIN                  GPIO_PIN(1, 1)    /**< User button, next to the USB-C port */
#define BTN0_MODE                 GPIO_IN_PU        /**< Button is active low, no external pull-up */
#define BTN0_INT_FLANK            GPIO_FALLING      /**< Interrupt fires on button press */
#define BTN1_PIN                  GPIO_PIN(1, 7)    /**< Capacitive touch button on the enclosure */
#define BTN1_MODE                 GPIO_IN_PU        /**< Assumed active low, same wiring as BTN0 */
#define BTN1_INT_FLANK            GPIO_FALLING      /**< Interrupt fires on touch */
/** @} */

/**
 * @name    LED (on-board) configuration
 * @{
 */
#define LED0_PIN                  GPIO_PIN(0, 15)       /**< User LED, driven high to light up */
#define LED0_ON                   gpio_set(LED0_PIN)    /**< Switch the user LED on */
#define LED0_OFF                  gpio_clear(LED0_PIN)  /**< Switch the user LED off */
#define LED0_TOGGLE               gpio_toggle(LED0_PIN) /**< Toggle the user LED */
#define LED1_PIN                  GPIO_PIN(0, 19)       /**< Mesh heartbeat LED, driven high to light up */
#define LED1_ON                   gpio_set(LED1_PIN)    /**< Switch the heartbeat LED on */
#define LED1_OFF                  gpio_clear(LED1_PIN)  /**< Switch the heartbeat LED off */
#define LED1_TOGGLE               gpio_toggle(LED1_PIN) /**< Toggle the heartbeat LED */
/** @} */

/**
 * @name    SX1262 (Wio-SX1262 module) control pins
 * @{
 */
#define SOLAR_LORA_DIO1_PIN       GPIO_PIN(0, 3)    /**< SX1262 DIO1, radio interrupt line */
#define SOLAR_LORA_CS_PIN         GPIO_PIN(0, 4)    /**< SX1262 NSS, SPI chip select (active low) */
#define SOLAR_LORA_SW_PIN         GPIO_PIN(0, 5)    /**< SX1262 RXEN, enables the RX path of the RF switch */
#define SOLAR_LORA_RESET_PIN      GPIO_PIN(0, 28)   /**< SX1262 NRESET, radio reset (active low) */
#define SOLAR_LORA_BUSY_PIN       GPIO_PIN(0, 29)   /**< SX1262 BUSY, high while the radio processes a command */
#define SOLAR_LORA_CLK_PIN        GPIO_PIN(1, 13)   /**< SX1262 SCK, SPI clock */
#define SOLAR_LORA_MISO_PIN       GPIO_PIN(1, 14)   /**< SX1262 MISO, SPI radio to MCU */
#define SOLAR_LORA_MOSI_PIN       GPIO_PIN(1, 15)   /**< SX1262 MOSI, SPI MCU to radio */
/** @} */

/**
 * @name    SX1262 driver configuration
 *
 * The TX path of the RF switch is driven by the radio itself through DIO2,
 * only the RX path is wired to the MCU (@ref SOLAR_LORA_SW_PIN). DIO3 supplies
 * the 1.8V rail of the module TCXO, without it the radio has no 32MHz clock.
 * @{
 */
#define SX126X_PARAM_SPI          SPI_DEV(0)            /**< SPI bus the radio sits on */
#define SX126X_PARAM_SPI_NSS      SOLAR_LORA_CS_PIN     /**< Chip select pin of the radio */
#define SX126X_PARAM_RESET        SOLAR_LORA_RESET_PIN  /**< Reset pin of the radio */
#define SX126X_PARAM_BUSY         SOLAR_LORA_BUSY_PIN   /**< Busy pin of the radio */
#define SX126X_PARAM_DIO1         SOLAR_LORA_DIO1_PIN   /**< Interrupt pin of the radio */
#define SX126X_PARAM_DIO2_MODE    SX126X_DIO2_RF_SWITCH /**< DIO2 drives the TX side of the RF switch */
#define SX126X_PARAM_DIO3_MODE    SX126X_DIO3_TCXO      /**< DIO3 powers the on-module TCXO */
#define SX126X_PARAM_TCXO_VOLTAGE SX126X_TCXO_CTRL_1_8V /**< TCXO supply voltage of the Wio-SX1262 */
#define SX126X_PARAM_TX_PA_MODE   SX126X_RF_MODE_TX_HPA /**< SX1262 has a high power amplifier (22dBm) */
#if IS_USED(MODULE_SX126X_RF_SWITCH)
/**
 * @brief   Drive @ref SOLAR_LORA_SW_PIN according to the requested RF mode
 *
 * @param[in] dev       radio device descriptor
 * @param[in] rf_mode   RF mode the radio is switching to
 */
void solar_p1_sx126x_set_rf_mode(sx126x_t *dev, sx126x_rf_mode_t rf_mode);
#  define SX126X_PARAM_SET_RF_MODE_CB solar_p1_sx126x_set_rf_mode /**< RF switch callback of this board */
#endif
/** @} */

/**
 * @name    GNSS (XIAO L76K) control pins
 *
 * Only populated on the P1-Pro variant, the pins are unconnected on the P1.
 * @{
 */
#define SOLAR_GPS_WAKEUP    GPIO_PIN(0, 2)  /**< GNSS standby pin, drive high to wake the module */
#define SOLAR_GPS_RESET_PIN GPIO_PIN(1, 3)  /**< GNSS reset pin (active low) */
#define SOLAR_GPS_EN_PIN    GPIO_PIN(1, 5)  /**< GNSS power switch, drive high to power the module */
#define SOLAR_GPS_TX        GPIO_PIN(1, 11) /**< GNSS UART, module to MCU, 9600 baud NMEA */
#define SOLAR_GPS_RX        GPIO_PIN(1, 12) /**< GNSS UART, MCU to module */
/** @} */

/**
 * @name    Grove connector pins
 *
 * These two lines are the NFC antenna pins of the nRF52840. They are released
 * by the stock bootloader, see the board documentation.
 * @{
 */
#define SOLAR_GROVE_D0      GPIO_PIN(0, 10) /**< Grove pin 1, I2C SCL, NFC2 by default */
#define SOLAR_GROVE_D1      GPIO_PIN(0, 9)  /**< Grove pin 2, I2C SDA, NFC1 by default */
/** @} */

/**
 * @name    Battery and charging detection pins
 * @{
 */
#define SOLAR_BAT_READ      GPIO_PIN(0, 14) /**< Battery divider switch, drive low to enable it */
#define SOLAR_BAT_ADC_PIN   GPIO_PIN(0, 31) /**< Battery voltage sense, AIN7, reading x 3.3 */
/** @} */

/**
 * @name    Board configuration
 *
 * These are only the defaults used when building without Kconfig, see Kconfig
 * for the descriptions.
 * @{
 */
#ifndef CONFIG_SOLAR_P1_ENABLE_BAT_SENSE
#  define CONFIG_SOLAR_P1_ENABLE_BAT_SENSE 1 /**< Enable the battery voltage divider at boot */
#endif
#ifndef CONFIG_SOLAR_P1_ENABLE_GNSS
#  define CONFIG_SOLAR_P1_ENABLE_GNSS 0 /**< Power the GNSS module at boot (P1-Pro only) */
#endif
/** @} */

/**
 * @name    On-board QSPI flash (P25Q16H, 2MB) pins
 *
 * ponytail: pins are documented but no MTD device is exposed yet. To add one,
 * mirror boards/common/seeedstudio-xiao-nrf52840/mtd.c, the flash chip is the
 * same and it is driven over a plain SPI bus there.
 * @{
 */
#define SOLAR_QSPI_SCLK GPIO_PIN(0, 21) /**< QSPI clock */
#define SOLAR_QSPI_CSN  GPIO_PIN(0, 25) /**< QSPI chip select (active low) */
#define SOLAR_QSPI_DI   GPIO_PIN(0, 20) /**< QSPI IO0, MOSI in single SPI mode */
#define SOLAR_QSPI_DO   GPIO_PIN(0, 24) /**< QSPI IO1, MISO in single SPI mode */
#define SOLAR_QSPI_WP   GPIO_PIN(0, 22) /**< QSPI IO2, write protect in single SPI mode */
#define SOLAR_QSPI_HOLD GPIO_PIN(0, 23) /**< QSPI IO3, hold in single SPI mode */
/** @} */

#ifdef __cplusplus
}
#endif

/** @} */
