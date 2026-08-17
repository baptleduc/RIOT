@defgroup   boards_seeedstudio-sensecap-solar-p1 SeeedStudio SenseCAP Solar Node P1
@ingroup    boards
@brief      Support for the SeeedStudio SenseCAP Solar Node P1
@author     Léandre Le Duc <leandre.leduc@etik.com>

No schematic is published for this board. Pin configuration is based on the
[Meshtastic firmware variant](https://github.com/meshtastic/firmware/tree/develop/variants/nrf52840/seeed_solar_node)
maintained by SeeedStudio for it.

## Overview

The [SenseCAP Solar Node P1](https://wiki.seeedstudio.com/meshtastic_solar_node/)
is a solar powered outdoor LoRa node, built around a XIAO nRF52840 Plus module
plugged into a carrier board:

- Nordic nRF52840 SoC @64MHz, 1MB Flash, 256KB RAM
- P25Q16H 2MB SPI NOR flash (on the XIAO module)
- Semtech SX1262 LoRa transceiver (Wio-SX1262 module, 862-930MHz, 22dBm)
- Quectel L76K GNSS module (P1-Pro variant only)
- Grove connector (I2C, GPIO or UART)
- 1 user button and 1 capacitive touch button
- 2 user LEDs
- 5W solar panel, CN3165 charger and 4 x 18650 battery slots
  (batteries are only shipped with the P1-Pro variant)

The P1 and the P1-Pro share the same carrier board, the P1 simply leaves the
GNSS module and the batteries unpopulated.

## Hardware

| MCU         | nRF52840                               | Supported |
|:----------- |:-------------------------------------- |:---------:|
| Family      | ARM Cortex-M4 with FPU                 |           |
| Vendor      | Nordic Semiconductor                   |           |
| RAM         | 256 KByte                              |           |
| Flash       | 1 MByte                                |           |
| Frequency   | 64 MHz                                 |           |
| FPU         | yes                                    |   yes     |
| Timers      | 5 (32 bit)                             |   yes     |
| RTC         | 3                                      |   yes     |
| ADC         | 12 bit SAADC, 8 channels               |   yes     |
| UART        | 2 x UARTE                              |   yes     |
| SPI         | 3 x SPIM                               |   yes     |
| I2C         | 2 x TWIM                               |   yes     |
| PWM         | 4 x PWM (4 ch each)                    |   yes     |
| USB         | 1 x Full Speed                         |   yes     |
| BLE         | Bluetooth 5.0                          |   yes     |
| IEEE 802.15.4 | yes                                  |   yes     |
| Vcc         | 1.7V - 3.6V                            |           |
| Datasheet   | [nRF52840 Product Specification](https://docs.nordicsemi.com/bundle/ps_nrf52840/page/keyfeatures_html5.html) | |
| Board Manual | [SenseCAP Solar Node Wiki](https://wiki.seeedstudio.com/meshtastic_solar_node/) | |

## Pin Layout / Configuration

| RIOT Peripheral      | MCU Pin | Board Function              | Remark                        |
|:---------------------|:--------|:----------------------------|:------------------------------|
| BTN0                 | P1.01   | User button                 | pull-up, active low           |
| BTN1                 | P1.07   | Touch button                | pull-up, active low           |
| LED0                 | P0.15   | User LED                    | active high                   |
| LED1                 | P0.19   | Mesh heartbeat LED          | active high                   |
| UART_DEV(0) RX       | P1.12   | GNSS (L76K) RX              | 9600 baud NMEA                |
| UART_DEV(0) TX       | P1.11   | GNSS (L76K) TX              |                               |
| SPI_DEV(0) SCLK      | P1.13   | SX1262 SCK                  |                               |
| SPI_DEV(0) MOSI      | P1.15   | SX1262 MOSI                 |                               |
| SPI_DEV(0) MISO      | P1.14   | SX1262 MISO                 |                               |
| I2C_DEV(0) SCL       | P0.10   | Grove pin 1                 | NFC2, see below               |
| I2C_DEV(0) SDA       | P0.09   | Grove pin 2                 | NFC1, see below               |
| NRF52_AIN7           | P0.31   | Battery voltage ADC         | multiply reading by 3.3       |
| SOLAR_LORA_CS_PIN    | P0.04   | SX1262 chip select          |                               |
| SOLAR_LORA_RESET_PIN | P0.28   | SX1262 reset                |                               |
| SOLAR_LORA_DIO1_PIN  | P0.03   | SX1262 IRQ (DIO1)           |                               |
| SOLAR_LORA_BUSY_PIN  | P0.29   | SX1262 busy                 |                               |
| SOLAR_LORA_SW_PIN    | P0.05   | RF switch, RX path          | TX path driven by DIO2        |
| SOLAR_GPS_EN_PIN     | P1.05   | GNSS power enable           |                               |
| SOLAR_GPS_RESET_PIN  | P1.03   | GNSS reset                  |                               |
| SOLAR_GPS_WAKEUP     | P0.02   | GNSS standby                |                               |
| SOLAR_BAT_READ       | P0.14   | Battery divider enable      | active low                    |
| SOLAR_QSPI_*         | P0.20-25| P25Q16H SPI NOR flash       | no MTD device exposed yet     |

## LoRa transceiver

Three details of the Wio-SX1262 module are not optional, the board configures
them through `SX126X_PARAM_DIO2_MODE`, `SX126X_PARAM_DIO3_MODE` and the
`sx126x_rf_switch` module:

- DIO3 supplies the 1.8V rail of the module TCXO. Without it the radio has no
  32MHz clock and does not start at all.
- DIO2 drives the TX path of the RF switch. Without it most of the transmitted
  power is lost in the switch.
- RXEN (`SOLAR_LORA_SW_PIN`) drives the RX path of the RF switch from the MCU,
  through the `set_rf_mode` callback implemented in `board.c`.

## Grove connector and NFC

The two Grove signal pins are the NFC antenna pins of the nRF52840, which the
hardware latches to the NFC peripheral at reset unless the `NFCPINS` field of
the UICR is cleared. Nothing needs to be done for that here: the stock
bootloader is built with `CONFIG_NFCT_PINS_AS_GPIOS`, so it clears the field
on first boot, long before RIOT runs.

@warning Erasing the whole chip over SWD also erases the UICR and brings the
NFC peripheral back, which leaves the Grove connector dead. Restoring the
bootloader restores the Grove connector as well.

## Flashing the Device

The Solar Node ships with the
[Adafruit nRF52 Bootloader](https://github.com/adafruit/Adafruit_nRF52_Bootloader),
you can find flashing instructions in the
[Adafruit nRF52 Bootloader](@ref boards_common_adafruit-nrf52-bootloader) section.

Double press the reset button to enter DFU mode, then run:

```shell
BOARD=seeedstudio-sensecap-solar-p1 make -C examples/basic/hello-world flash
```

@note Accessing the SWD pads requires opening the enclosure. Overwriting the
bootloader via SWD would make UF2 flashing unavailable and is not recommended
for normal use.

## Accessing STDIO

`stdio` is provided by the USB CDC ACM interface over the USB-C connector, no
UART is broken out. `UART_DEV(0)` is wired to the GNSS module.

```shell
BOARD=seeedstudio-sensecap-solar-p1 make -C examples/basic/hello-world flash term
```
