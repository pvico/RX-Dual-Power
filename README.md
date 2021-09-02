# RX-Dual-Battery-Switch-R2.0

## Description
TODO


## Types of power sources

- BEC from 5 to 8.4V
- LIPO 2-4S (7.4 to 14.8V)
- LIFE 2-4S (6.6 to 13.2V)
- NIMH 4-5S (4.8 to 6V)

Maximum voltage is 16.8V (fully charged LIPO 4S), minimum is 5V (BEC 5V or NIMH 4S).
The MCU will remain powered down to about 4V. The RX and servos will remain powered down to an available voltage of 2.5V. However, most receivers and/or servos will have failed before reaching this.

Whatever voltages are present at the power source inputs can be present at the output. So, if one or both of the power source voltages is above the maximum voltage of the RX or servos, a BEC must be placed *after* the RX Dual Battery Switch and *both* power source voltages must be sufficient to drive that BEC.

## Strategies

One of the following power source selection strategies must be selected:

1. Use main power source as long as it is available and of sufficient voltage, otherwise use standby power source
2. Use the power source with the highest voltage (if 2 batteries are used as power source, they will be selected alternativeley, whichever is 20mV above the other will be selected)

For strategy 1, the MCU determines the power source. For strategy 2, the MCU will let the LTC4412's control the power source.

Note: if strategy 1 is selected and MAIN and STBY power sources are both low voltage or one is disconnected, then strategy 2 is applied.

Should the MCU become unpowered, e.g. if available voltage is too low to keep the 3.3V regulator powered, then strategy 2 is applied.

## Powering off the model

To power off the RX and servos, use a neodymium magnet of sufficient size and power ([e.g. this one](https://www.amazon.de/-/en/Magnetpro-Countersunk-Magnet-Cushions-Capsule/dp/B08K39Q1DL/ref=pd_sbs_1/261-1102478-9650911?pd_rd_w=4NK6S&pf_rd_p=b1c388c3-48c2-4960-8532-fa8f1477aee9&pf_rd_r=2AJZ6JFC8H0XXN0D8038&pd_rd_r=500284af-6c54-4b1d-af8f-a95a1c957906&pd_rd_wg=SNuGS&pd_rd_i=B08K39Q1DL&psc=1)).

Approaching the magnet 2 times close to the AH180 on the PCB will power off the model. When the model is powered off, approaching the magnet once will power the model back on. The AH180 (U5) is clearly indicated on the PCB with a label 'MAGNET' and an arrow. 

The magnet shown above will be detected when it is about 2.5cm (1") either *above* or *below* the AH180. Position the RX Dual Battery Switch appropriately in the model.

Alternatively, if you don't have the magnet, press both buttons (SW1 and SW2) simultaneously for 2" to power off. Press any one button to power on. 

## LED signalling

|                       | Green (LED1)  | Yellow (LED2) |
| :-------------------- | :----------:  | :-----------: |
| Using MAIN power      |      DIM      |      OFF      |
| Using STBY power      |      OFF      |   BLINK SLOW  |
| Power off             |      OFF      |      OFF      |
| Magnet detected       |       x       |   BLINK FAST  |

## S.Port reporting

- Using stby power (only for strategy 1)
- stby power low (only if )
- main power low (only for strategy 2)
- main power disconnected
- stby power disconnected
- main power source voltage
- stby power source voltage

Non-standard S.Port sensor id's or messages are used, so adequate programming of the transmitter using OpenTX must be done to have vocal messages corresponding to these reportings.

If using a non-OpenTX transmitter, e.g., FrSky Tandem X20, an adequate message must be configured in response to the main power source voltage reporting.
