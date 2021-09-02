# RX Dual Battery Switch

## Description

**TODO**

## Types of power sources

The main power source (labelled MAIN PWR on the back of the PCB) and the standby power source (labelled STBY PWR on the back of the PCB) can be any of the following:

- BEC from 5 to 8.4V
- LIPO 2-4S (7.4 to 14.8V)
- LIFE 2-4S (6.6 to 13.2V)
- NIMH 4-5S (4.8 to 6V)

Maximum allowed voltage is 16.8V (fully charged LIPO 4S). The minimum source voltage should be at least 5V (4.8V is ok for NIMH 2S).

With the available voltage decreasing due to battery discharge, the MCU will remain powered until the available voltage reaches about 4V. Even if the MCU has powered down due to low voltage, the RX and servos will still remain powered until a voltage of 2.5V is reached. However, most receivers and/or servos will have failed before reaching this low voltage.

Whatever voltages are present at the power source inputs can be present at the output. So, if one or both of the power source voltages is above the maximum voltage of the receiver or servos, *a BEC must be placed **after** the RX Dual Battery Switch and both power source voltages must be sufficient to drive that BEC*.

## Power source selection strategy

The system must be configured for one of the following power source selection strategies:

1. Use MAIN PWR as long as it is available and of sufficient voltage, otherwise use STBY PWR
2. Use the power source with the highest voltage

### System configured for strategy #1

As long as MAIN PWR is above its minimum voltage (according to its type), the MCU selects MAIN PWR and forces STBY PWR off<sup>1</sup>.

When MAIN PWR is *below its minimum voltage*<sup>2</sup> - but **not** disconnected or in short-circuit - STBY PWR powers the model<sup>3</sup>.

If MAIN PWR and STBY PWR are **both** *below minimum voltage*<sup>1</sup>  - or if *any one* is disconnected or in short-circuit -  strategy #2 is applied<sup>4</sup>, the source with the highest voltage now powers the model, the other one becoming isolated from the system.

<sub><sup>1</sup>By setting the CTL2 line to HIGH and releasing the CTL1 line (MCU pin set to high impedance).</sub><br/>
<sub><sup>2</sup>Below minimum voltage only applies to a battery. A BEC is never below minimum voltage: it is either above minimum voltage (> 4.8V) or considered disconnected (< 4.8V).</sub><br/>
<sub><sup>3</sup>The MCU releases the CTL2 line and sets the CTL1 line to HIGH, forcing MAIN PWR off.</sub><br/>
<sub><sup>4</sup>Both CTL1 and CTL2 lines are released (MCU pin set to high impedance).</sub><br/>

### System configured for strategy #2

The MCU will let the LTC4412's control the power source<sup>5</sup>. 

The LTC4412's will select the source with the highest voltage and isolate the other one from the sytem.

Note: if two batteries of the same type are used as power source, they will be selected alternativeley, whichever one is 20mV above the other will power the model. They will discharge in parallel.

<sub><sup>5</sup>Both CTL1 and CTL2 lines are released (MCU pin set to high impedance).</sub><br/>

### If the MCU is powered down due to low volatge

Should the MCU become unpowered, e.g. if the available voltage becomes too low to keep the 3.3V regulator powered, strategy #2 is applied<sup>6</sup.

<sub><sup>6</sup>Both CTL1 and CTL2 lines are set to high impedance just before the MCU powers down (brownout detection).</sub><br/>

## Typical configurations

**TODO**

## Powering off the model

To power off the RX and servos, use a neodymium magnet of sufficient size and power ([e.g. this one](https://www.amazon.de/-/en/Magnetpro-Countersunk-Magnet-Cushions-Capsule/dp/B08K39Q1DL/ref=pd_sbs_1/261-1102478-9650911?pd_rd_w=4NK6S&pf_rd_p=b1c388c3-48c2-4960-8532-fa8f1477aee9&pf_rd_r=2AJZ6JFC8H0XXN0D8038&pd_rd_r=500284af-6c54-4b1d-af8f-a95a1c957906&pd_rd_wg=SNuGS&pd_rd_i=B08K39Q1DL&psc=1), 20 x 7mm, 10kg force).

Approaching the magnet 2 times close to the AH180 hall effect sensor on the PCB will power off the model. When the model is powered off, approaching the magnet once will power the model back on. The AH180 (U5) is clearly indicated on the PCB with a label 'MAGNET' and an arrow. 

The magnet shown above will be detected when it is about 2.5cm (1") either *directly above* or *directly below* the AH180 (not on the side). Position the RX Dual Battery Switch PCB appropriately in the model to be able to power off/on without opening any canopy or cover.

Alternatively, if you don't have a magnet, press both buttons (SW1 and SW2) simultaneously for 2" to power off. Press any button to power on. 

Powering off the model is achieved by the MCU setting both CTL1 and CTL2 lines to high.

When the model is powered off, the current consumed should be minimal (less than 100µA).

## S.Port reporting

### Values

- MAIN PWR voltage
- STBY PWR voltage

These are transmitted using sensor ID: **TBD**

### Messages

| Message               | Condition                                                                        |
| :-------------------: | :------------------------------------------------------------------------------- |
| USING MAIN PWR        | System configured for strategy #1 is selected and MAIN PWR is the power source   |
| USING STBY PWR        | System configured for strategy #1 selected and STBY PWR is the power source      |
| STBY PWR LOW          | STBY PWR is a battery and it is below its minimum voltage                        |
| MAIN PWR LOW          | MAIN PWR is a battery and it is below its minimum voltage                        |
| MAIN PWR DISCONNECTED | MAIN PWR is below 0.5V                                                           |
| STBY PWR DISCONNECTED | STBY PWR is below 0.5V                                                           |
| CRITICAL              | Both MAIN PWR and STBY PWR are either LOW or DISCONNECTED                        |

Note: these are non standard ad-hoc messages transmitted using sensor ID: **TBD**.
Adequate programming of the transmitter using OpenTX must be done to have audio and/or visual messages corresponding to each reported message.

If using a non-OpenTX transmitter, e.g., FrSky Tandem X20, an adequate audio and/or visual message must be configured in response to the MAIN PWR voltage value reporting.

## LED signalling

| Condition*                                         | Green (LED1)  | Yellow (LED2) |
| -------------------------------------------------- | :----------:  | :-----------: |
| Power off                                          |      OFF      |      OFF      |
| Magnet detected                                    |       x       |     BRIGHT    |
| MAIN PWR LOW or DISCONNECTED                       |   BLINK FAST  |       x       |
| STBY PWR LOW or DISCONNECTED                       |       x       |   BLINK FAST  |
| Using STBY PWR (system configured for strategy #1) |       x       |   BLINK SLOW  |
| Using STBY PWR (system configured for strategy #2) |       x       |      DIM      |
| Using MAIN PWR                                     |      DIM      |       x       |

\* Listed in order of priority. For example, if both MAIN PWR and STBY PWR are LOW or DISCONNECTED (CRITICAL condition), LED1 and LED2 will both blink fast whichever source is powering the model.

## System configuration

In config.h and re-compiling or by programming through the S.Port connector: **TBD**