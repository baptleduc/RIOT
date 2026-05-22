/*
 * SPDX-FileCopyrightText: 2025 Baptiste Le Duc <baptiste.leduc@etik.com>
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#pragma once

/**
 * @ingroup     drivers_qma6100p
 * @{
 *
 * @file
 * @brief       Register definitions for the QST QMA6100P accelerometer
 *
 * @author      Baptiste Le Duc <baptiste.leduc@etik.com>
 * @author      Léandre Le Duc <leandre.leduc38@gmail.com>
 */

#ifdef __cplusplus
extern "C" {
#endif

#define BIT(n)                 (1UL << (n))
#define GENMASK(h, l)          (((1UL << ((h) - (l) + 1)) - 1) << (l))

#define FIELD_CLEAR(mask, reg) ((reg) &= ~(mask))
#define FIELD_PREP(mask, val)  (((val) << __builtin_ctz(mask)) & (mask))
#define FIELD_SET(mask, val, reg)       \
    do {                                \
        FIELD_CLEAR(mask, reg);         \
        (reg) |= FIELD_PREP(mask, val); \
    } while (0)

/**
 * @name    Register addresses (QST-PD-B002-22 Table 14 / section 9)
 * @{
 */
#define QMA6100P_REG_CHIP_ID             (0x00) /**< Chip ID */
/* Acceleration data — 14-bit two's complement, burst-readable 0x01–0x06.
 * LSB reg: bits[7:2]=data[5:0], bit[0]=NEWDATA flag (1=updated since last read).
 * MSB reg: bits[7:0]=data[13:6], bit[7] is the sign bit. */
#define QMA6100P_REG_DX_LSB              (0x01) /**< X low: data[5:0] in bits[7:2] */
#define QMA6100P_REG_DX_MSB              (0x02) /**< X high: data[13:6] */
#define QMA6100P_REG_DY_LSB              (0x03) /**< Y low */
#define QMA6100P_REG_DY_MSB              (0x04) /**< Y high */
#define QMA6100P_REG_DZ_LSB              (0x05) /**< Z low */
#define QMA6100P_REG_DZ_MSB              (0x06) /**< Z high */
#define QMA6100P_REG_INT_ST2             (0x0B) /**< Interrupt status 2 (FIFO/data) */
#define QMA6100P_REG_RANGE               (0x0F) /**< Full-scale range + LPF/HPF select */
#define QMA6100P_REG_ODR                 (0x10) /**< Output data rate + NLPF */
#define QMA6100P_REG_PM                  (0x11) /**< Power management (MODE, MCLK) */
#define QMA6100P_REG_INT_EN1             (0x17) /**< Interrupt enable register */
#define QMA6100P_REG_INT1_MAP1           (0x1A) /**< INT1 map: no_mot/FIFO/data/q_tap/any_mot */
#define QMA6100P_REG_INT2_MAP1           (0x1C) /**< INT2 map: no_mot/FIFO/data/q_tap/any_mot */
#define QMA6100P_REG_INTPIN_CONF         (0x20) /**< INT pin config (OD/LVL) */
#define QMA6100P_REG_INT_CFG             (0x21) /**< Interrupt latch/shadow/I2C config */
#define QMA6100P_REG_SW_RESET            (0x36) /**< Soft reset */
#define QMA6100P_REG_NVM                 (0x33) /**< Non Volatile Memory */
#define QMA6100P_REG_CHIP_STATE          (0x45) /**< Chip state */
#define QMA6100P_REG_ULPS                (0x46) /**< ULPS control register */
#define QMA6100P_REG_TST0_ANA            (0x4A) /**< Analog test register 0 */
/** @} */

/**
 * @name    CHIP_ID (0x00)
 * Bits[7:4] = 0x9 (fixed). Bits[3:0] factory-set, software must ignore.
 * @{
 */
#define QMA6100P_CHIP_ID_MASK            (0xF0)
#define QMA6100P_CHIP_ID_VAL             (0x90)
/** @} */

/**
 * @name    Data LSB registers (0x01 / 0x03 / 0x05)
 * @{
 */
#define QMA6100P_NEWDATA_FLAG            BIT(0) /**< 1 = channel updated since last read */
/** @} */

/**
 * @name    INT_ST2 (0x0B) — data-ready status
 * @{
 */
#define QMA6100P_INT_ST2_DATA_INT        BIT(4) /**< data-ready interrupt active */
/** @} */

/**
 * @name    RANGE (0x0F) — full-scale range and filter select
 * @{
 */
#define QMA6100P_RANGE_MASK              GENMASK(3, 0)
/* bits[5:4] RESERVED, must be 0 */
#define QMA6100P_RANGE_LPF_HPF           BIT(6) /**< 0=LPF mode, 1=HPF mode (affects NLPF in ODR) */
/** @} */

/**
 * @name    ODR (0x10) — output data rate and filter
 * NLPF[2:0] in bits[7:5], ODR[4:0] in bits[4:0].
 * @{
 */
#define QMA6100P_ODR_MASK                GENMASK(4, 0) /**< ODR bits[4:0] mask */
#define QMA6100P_NLPF_SHIFT              (5)           /**< NLPF field shift */
#define QMA6100P_NLPF_MASK               GENMASK(7, 5) /**< NLPF bits[7:5] mask */
/* NLPF when RANGE[LPF_HPF]=0 (LPF mode): 000=off, 100=1, x01=2, x10=4, x11=8 */
/* NLPF when RANGE[LPF_HPF]=1 (HPF mode): 000=off, 001=ODR/10, 010=ODR/25, etc. */
/** @} */

/**
 * @name    PM (0x11) — power management
 * Default 0x00 after POR — device is in standby. Write MODE=1 to activate.
 * @{
 */
#define QMA6100P_PM_MODE_SHIFT           (7)                         /**< shift for mode bit */
#define QMA6100P_PM_MODE_MASK            BIT(QMA6100P_PM_MODE_SHIFT) /**< mask for mode bit */
/* bit6 RESERVED */
#define QMA6100P_PM_SINC_SHIFT           (4)                              /**< shift for T_RSTB_SINC[1:0] */
#define QMA6100P_PM_SINC_MASK            GENMASK(5, 4)                    /**< mask for T_RSTB_SINC[1:0] */
#define QMA6100P_PM_SINC_3MCLK           (0x00 << QMA6100P_PM_SINC_SHIFT) /**< SINC reset pulse = 3 MCLK cycles */
#define QMA6100P_PM_SINC_4MCLK           (0x01 << QMA6100P_PM_SINC_SHIFT) /**< SINC reset pulse = 4 MCLK cycles */
#define QMA6100P_PM_SINC_6MCLK           (0x02 << QMA6100P_PM_SINC_SHIFT) /**< SINC reset pulse = 6 MCLK cycles */
#define QMA6100P_PM_SINC_8MCLK           (0x03 << QMA6100P_PM_SINC_SHIFT) /**< SINC reset pulse = 8 MCLK cycles */
#define QMA6100P_PM_MCLK_MASK            GENMASK(3, 0)                    /**< MCLK[3:0] */
#define QMA6100P_PM_MCLK_51K2            (0x04)                           /**< 51.2 kHz — recommended default */
#define QMA6100P_PM_MCLK_25K6            (0x05)
#define QMA6100P_PM_MCLK_12K8            (0x06)
#define QMA6100P_PM_MCLK_6K4             (0x07)
/** @} */

/**
 * @name    INT_EN1 (0x17) — data-ready interrupt enable
 * @{
 */
#define QMA6100P_INT_EN1_DATA            BIT(4) /**< data-ready interrupt enable */
/** @} */

/**
 * @name    INT1_MAP1 (0x1A) / INT2_MAP1 (0x1C) — route data-ready to INT pin
 * @{
 */
#define QMA6100P_INT_MAP1_DATA           BIT(4) /**< route data-ready to INTx pin */
/** @} */

/**
 * @name    INTPIN_CONF (0x20) — INT pin electrical config
 * Default 0x05 = INT1 active-high push-pull, INT2 active-high push-pull.
 * @{
 */
#define QMA6100P_INTPIN_INT1_LVL         BIT(0) /**< 0=active-low, 1=active-high */
#define QMA6100P_INTPIN_INT1_OD          BIT(1) /**< 0=push-pull, 1=open-drain */
#define QMA6100P_INTPIN_INT2_LVL         BIT(2) /**< 0=active-low, 1=active-high */
#define QMA6100P_INTPIN_INT2_OD          BIT(3) /**< 0=push-pull, 1=open-drain */

/**
 * @name   INT pin active level
 * @{
 */
#define QMA6100P_INTPIN_ACTIVE_LOW       (0) /**< INT pin active LOW on interrupt */
#define QMA6100P_INTPIN_ACTIVE_HIGH      (1) /**< INT pin active HIGH on interrupt */
/** @} */
/** @} */

/**
 * @name   INT status clear behavior
 * @{
 */
#define QMA6100P_INT_CFG_CLR_SHIFT       (7)
#define QMA6100P_INT_CFG_CLR_MASK        (BIT(QMA6100P_INT_CFG_CLR_SHIFT))
#define QMA6100P_INT_CFG_CLR_ON_LATCH    (0) /**< INT_STATUS bits cleared only if latched */
#define QMA6100P_INT_CFG_CLR_ON_ANY_READ (1) /**< INT_STATUS bits cleared on any read */
/** @} */

/**
 * @name   INT latch mode
 * @{
 */
#define QMA6100P_INT_CFG_LATCH_SHIFT     (0)
#define QMA6100P_INT_CFG_LATCH_MASK      (BIT(QMA6100P_INT_CFG_LATCH_SHIFT))
#define QMA6100P_INT_CFG_NON_LATCH       (0) /**< INT pulse clears automatically */
#define QMA6100P_INT_CFG_LATCH           (1) /**< INT held until ack via @ref qma6100p_ack_int */
/** @} */

/**
 * @name   INT shadowing function mode
 * @{
 */

#define QMA6100P_INT_CFG_SHADOW_SHIFT    (6)                                /**< bit position of shadow disable bit */
#define QMA6100P_INT_CFG_SHADOW_MASK     BIT(QMA6100P_INT_CFG_SHADOW_SHIFT) /**< shadow disable bit mask */
#define QMA6100P_INT_CFG_SHADOW_EN       (0)                                /**< shadowing enabled (default) */
#define QMA6100P_INT_CFG_SHADOW_DIS      (1)                                /**< shadowing disabled */
/** @} */

/**
 * @name    SW_RESET (0x36)
 * @{
 */
#define QMA6100P_SW_RESET_VAL            (0xB6) /**< trigger soft reset */
/** @} */

#ifdef __cplusplus
}
#endif

/** @} */
