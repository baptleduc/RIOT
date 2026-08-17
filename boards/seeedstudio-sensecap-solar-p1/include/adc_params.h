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
 * @brief       Configuration of SAUL mapped ADC lines
 *
 * @author      Léandre Le Duc <leandre.leduc@etik.com>
 */

#include "periph/adc.h"
#include "saul/periph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   ADC SAUL configuration
 *
 * @note    BAT exposes the raw output of the on-board voltage divider, not a
 *          voltage. Scaling it back to the battery voltage is left to the
 *          application, see the board documentation.
 *
 * @note    The divider has to be enabled to read anything meaningful, see
 *          CONFIG_SOLAR_P1_ENABLE_BAT_SENSE.
 */
static const saul_adc_params_t saul_adc_params[] = {
    {
        .name = "BAT",
        .line = ADC_LINE(NRF52_AIN7), /* P0.31 */
        .res = ADC_RES_10BIT,
    },
};

#ifdef __cplusplus
}
#endif

/** @} */
