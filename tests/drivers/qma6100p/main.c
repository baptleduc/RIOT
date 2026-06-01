/*
 * SPDX-FileCopyrightText: 2026 Baptiste Le Duc <baptiste.leduc@etik.com>
 * SPDX-License-Identifier: LGPL-2.1-only
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Test application for the QMA6100P accelerometer driver
 *
 * @author      Baptiste Le Duc <baptiste.leduc@etik.com>
 *
 * @}
 */

#include <inttypes.h>
#include <stdio.h>

#include "qma6100p.h"

/* verification: lower ODR to prove the data-ready ISR is locked to the data rate */

static const qma6100p_odr_t rates[] = {
    QMA6100P_ODR_12HZ5,
    QMA6100P_ODR_25HZ,
    QMA6100P_ODR_100HZ,
};
static const unsigned expect_hz[] = { 12, 25, 100 };

#define QMA6100P_PARAM_RATE rates[0]

#include "qma6100p_params.h"
#include "ztimer.h"

#define SLEEP_S (5U)

/* incremented in ISR context on every data-ready interrupt */
static volatile unsigned irq_count;

static void callback(void *args)
{
    (void)args;
    irq_count++;
}

static inline unsigned int _measure_irq_hz(void)
{
    unsigned before = irq_count;
    ztimer_sleep(ZTIMER_SEC, 1);
    return irq_count - before;
}

static qma6100p_t dev;
static const qma6100p_int_t interrupt = { .params = qma6100p_int_params[0], .cb = callback };

int main(void)
{
    int res;

    ztimer_sleep(ZTIMER_SEC, SLEEP_S);
    puts("=== QMA6100P accelerometer driver test ===");
    printf("[init] I2C_DEV(%d) addr 0x%02x ... ",
           (int)qma6100p_params[0].i2c, qma6100p_params[0].addr);

    res = qma6100p_init(&dev, &qma6100p_params[0]);
    if (res < 0) {
        printf("FAILED (%d)\n", res);
        return 1;
    }
    puts("OK");

    assert(dev.params.rate == QMA6100P_ODR_12HZ5);

    puts("\n--- data-ready interrupt rate sweep ---");
    for (unsigned int i = 0; i < ARRAY_SIZE(rates); i++) {
        unsigned int try;
        qma6100p_params_t p = *qma6100p_params; /* mutable copy */
        p.rate = rates[i];

        printf("[ODR %u/%u] set rate -> %u Hz ... ",
               i + 1, (unsigned)ARRAY_SIZE(rates), expect_hz[i]);

        res = qma6100p_init(&dev, &p);
        if (res < 0 || dev.params.rate != rates[i]) {
            printf("FAILED to apply rate (res=%d)\n", res);
            res = -1;
            goto out;
        }

        res = qma6100p_set_data_ready_int(&dev, &interrupt);
        if (res < 0) {
            printf("FAILED to enable data-ready int (res=%d)\n", res);
            goto out;
        }
        puts("OK");

        res = -1;
        for (try = 0; try < 3; try++) {
            unsigned hz = _measure_irq_hz();
            bool pass = (expect_hz[i] - 1 <= hz && hz <= expect_hz[i] + 1);
            printf("  try %u/3: measured %u Hz (expect ~%u) -> %s\n",
                   try + 1, hz, expect_hz[i], pass ? "PASS" : "off");
            if (pass) {
                res = 0;
                break;
            }
        }

        if (res < 0) {
            printf("[ODR %u/%u] FAILED: rate never matched ~%u Hz\n",
                   i + 1, (unsigned)ARRAY_SIZE(rates), expect_hz[i]);
            goto out;
        }
        printf("[ODR %u/%u] PASS (~%u Hz)\n",
               i + 1, (unsigned)ARRAY_SIZE(rates), expect_hz[i]);
    }

    puts("\n=== ALL TESTS PASSED ===");
    return 0;

out:
    puts("\n=== TEST FAILED ===");
    return res;
}
