# UART communication between progranmming device and target device

# Protocol

The UART communication between the progranmming device and the target device is at 9600bds 8N1 (the amount of data to transmit is minimal, a few bytes).

The target device listens on its telemetry or IN port for programming information during the first few ms after power up. Past these first ms, if no programming device is detected, the target device will configure for its normal UART parameters (e.g. SBUS IN in or S.Port in/out).

The progranmming device powers up the target device and repetitively sends a 4 bytes 'PROG' message. The target device reacts by replying with a 8 bytes message: 4 bytes ID ('RXDP', 'SVDC', etc.) followed by its current configuration coded on 32 bits. The UI terminal will then only enable the part of the UI designed for that type of target device and show the current config on it.

When, after modifying the configuration, the user depresses the 'Write' button on the UI, an 8 byte message is sent to the target device: 'WRIT' followed by the new config on 32 bits. The target device then sends 'CNFM' followed by the 32 bit new configuration written in its EEPROM.

The user simply disconnects the target device from the progranmming device to end the configuration. By so doing, the target device is powered off and will do a normal power-up sequence next time it is connected to power.

# Hardware

## Requirements

A single device is used for programming all target devices (RX Dual Power, Servo Decoder, future devices) from any user interface terminal (Windows PC, Mac, iOS device, Android device) and will:

    - be able to use USB communication with the Windows PC or Mac
        - a Windows or Mac OS app is used on the computer
    - be able to act as a wifi access point and publish a web interface for configuration with a smartphone
    - be based on a wifi and USB capable MCU
    - be powered through a micro-USB plug (5V, same as for communication with the PC) or common 2S RC battery
    - have an on-board 3.3V buck converter
    - be able to control the powering up of the target device
    - power the target device with 4.5V up to 8V (target devices are equipped with a 3.3V LDO)
    - be connected to the target device through a common 3-wire servo cable (half-duplex UART) and be the master of the communication
        - an external pull-up resistor is needed on the progranmming device half-duplex pin

## Selected MCU

The ESP32-S2 can be both a wifi access point and implement a USB CDC device