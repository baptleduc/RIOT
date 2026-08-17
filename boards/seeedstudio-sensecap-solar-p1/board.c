/*
 * SPDX-FileCopyrightText: 2026 Léandre Le Duc <leandre.leduc@etik.com>
 * SPDX-License-Identifier: LGPL-2.1-only
 */

/**
 * @ingroup     boards_seeedstudio-sensecap-solar-p1
 * @{
 *
 * @file
 * @brief       Board initialization for the SeeedStudio SenseCAP Solar Node P1
 *
 * @author      Léandre Le Duc <leandre.leduc@etik.com>
 *
 * @}
 */

#include "board.h"
#include "kernel_defines.h"
#include "periph/gpio.h"

#if IS_USED(MODULE_SX126X_RF_SWITCH)
#  include "sx126x.h"
#endif

void board_init(void)
{
    if (IS_ACTIVE(CONFIG_SOLAR_P1_ENABLE_BAT_SENSE)) {
        /* the divider is switched to ground, driving the pin low enables it */
        gpio_init(SOLAR_BAT_READ, GPIO_OUT);
        gpio_clear(SOLAR_BAT_READ);
    }

    /* keep the GNSS module powered down unless it is explicitly asked for */
    gpio_init(SOLAR_GPS_EN_PIN, GPIO_OUT);
    if (IS_ACTIVE(CONFIG_SOLAR_P1_ENABLE_GNSS)) {
        gpio_set(SOLAR_GPS_EN_PIN);
    }
    else {
        gpio_clear(SOLAR_GPS_EN_PIN);
    }

    if (IS_USED(MODULE_SX126X_RF_SWITCH)) {
        /* RX path of the RF switch, the TX path is driven by the radio itself
         * through DIO2 */
        gpio_init(SOLAR_LORA_SW_PIN, GPIO_OUT);
        gpio_clear(SOLAR_LORA_SW_PIN);
    }
}

#if IS_USED(MODULE_SX126X_RF_SWITCH)
void solar_p1_sx126x_set_rf_mode(sx126x_t *dev, sx126x_rf_mode_t rf_mode)
{
    (void)dev;

    switch (rf_mode) {
    case SX126X_RF_MODE_RX:
        gpio_set(SOLAR_LORA_SW_PIN);
        break;
    case SX126X_RF_MODE_TX_LPA:
    case SX126X_RF_MODE_TX_HPA:
        gpio_clear(SOLAR_LORA_SW_PIN);
        break;
    }
}
#endif
