# RX Dual Battery Switch

## Description

**TODO**

## Types of power sources

The main power source (labelled MAIN PWR on the back of the PCB) and the standby power source (labelled STBY PWR on the back of the PCB) can be any of the following:

- BEC from 5 to 8.4V
- LIPO 2-4S (7.4 to 14.8V nominal, 8.4 to 16.8V fully charged)
- LIFE 2-4S (6.6 to 13.2V nominal, 7 to 14V fully charged)
- NIMH 4-5S (4.8 to 6V nominal, not recommended)

The maximum allowed voltage is 16.8V (fully charged LIPO 4S). The minimum source voltage is 5V. A NIMH 4S (4.8V) can be used but NIMH batteries are generally not recommended55 as they tend to be unreliable.

Any voltage present at the power source inputs can be present at the output. So, if **one or both** of the power source voltages is above the maximum voltage of the receiver or servos, *a BEC must be placed **after** the RX Dual Battery Switch and **both** power source voltages must be sufficient to drive that BEC*.

Even with the *available voltage*<sup>1</sup> decreasing due to battery discharge, the receiver and servos will remain powered all the time until this voltage becomes so low that the receiver and/or servos fail.
##  

<sub><sup>1</sup>The available voltage is the highest of MAIN PWR voltage and STBY PWR voltage. The HT7533 voltage regulator delivering the 3.3V supply to the STM32 MCU is powered by this voltage less a diode drop. If the available voltage drops below 5V, the MCU VDD supply will gradually become unregulated but will initially remain above 3.3V. When the available voltage drops below about 4V, the MCU VDD will drop below 3.3V and the power source voltage measurements will become invalid. Anyway, by that time, the MCU will have switched to strategy #2 and whichever power source has the highest voltage will power the receiver and servos. When the available voltage drops below about 2.5V, the MCU will shut down and the LTC4412's behaviour is uncertain but with such a low voltage most receivers and servos will have failed.</sub><br/>

## Power source selection strategy

The system can be configured to use one of the following strategies for the selection of the power source:

- **Strategy #1**: Use MAIN PWR as long as it is available and of sufficient voltage, otherwise use STBY PWR
- **Strategy #2**: Use the power source with the highest voltage

### System configured for strategy #1

As long as MAIN PWR is above its minimum voltage (according to its type), the MCU selects MAIN PWR and forces STBY PWR off<sup>2</sup>.

When MAIN PWR is *below its minimum voltage*<sup>3</sup> - but **not** disconnected or in short-circuit - the MCU selects STBY PWR and forces MAIN PWR off<sup>4</sup>.

If MAIN PWR and STBY PWR are **both** below minimum voltage  - or if **any one** is disconnected or in short-circuit -  strategy #2 is applied and the source with the highest voltage now powers the model, the other one becoming isolated from the system.

### System configured for strategy #2

The MCU lets the LTC4412's control the power source<sup>5</sup>. 

The LTC4412's select the source with the highest voltage and isolate the other one from the sytem.

Note: if two batteries of the same type are used as power source, they will be selected alternatively<sup>6</sup> and will discharge in parallel.

##  

<sub><sup>2</sup>By setting the CTL2 line to HIGH and releasing the CTL1 line (MCU pin set to high impedance).</sub><br/>
<sub><sup>3</sup>Below minimum voltage only applies to a battery. A BEC is never below minimum voltage: it is either above minimum voltage (> 4.8V) or considered disconnected (< 4.8V).</sub><br/>
<sub><sup>4</sup>The MCU releases the CTL2 line and sets the CTL1 line to HIGH, forcing MAIN PWR off. Note that we have to do this instead of applying strategy #2 because MAIN PWR below its minimum voltage can still be above the STBY PWR voltage: consider the case of a discharged LIPO 2S as MAIN PWR (< 7.2V) and a fully charged LIFE 2S (7V) or NIMH 4S (5.4V) as STBY PWR .</sub><br/>
<sub><sup>5</sup>Both CTL1 and CTL2 lines are released (MCU pins set to high impedance).</sub><br/>
<sub><sup>6</sup>Whichever one is 20mV above the other will power the model.</sub><br/>

## Typical configurations

**TODO**

## Powering off the model

To power off the RX and servos, use a neodymium magnet of sufficient size and power ([e.g. this one](https://www.amazon.de/-/en/Magnetpro-Countersunk-Magnet-Cushions-Capsule/dp/B08K39Q1DL/ref=pd_sbs_1/261-1102478-9650911?pd_rd_w=4NK6S&pf_rd_p=b1c388c3-48c2-4960-8532-fa8f1477aee9&pf_rd_r=2AJZ6JFC8H0XXN0D8038&pd_rd_r=500284af-6c54-4b1d-af8f-a95a1c957906&pd_rd_wg=SNuGS&pd_rd_i=B08K39Q1DL&psc=1), 20 x 7mm, 10kg force).

Approaching the magnet 2 times within 2 seconds close to the hall effect sensor on the PCB will power off the model. When the model is powered off, approaching the magnet once will power the model back on. The hall effect sensor location is clearly indicated on the PCB with a label 'MAGNET' and an arrow. 

The magnet shown above will be detected when it is about 2.5cm (1") either *directly above* or *directly below* the hall effect sensor *but not on its side*. Position the RX Dual Battery Switch PCB appropriately in the model to be able to power off/on without opening any canopy or cover.

Alternatively, if you don't have a magnet, press both buttons (SW1 and SW2) simultaneously for 2" to power off the model<sup>8</sup>. Press any button to power it back on.

Connecting a power source, e.g. a new battery, will also power on the model.

When the model is powered off, the current consumed is minimal<sup>9</sup>.

##  

<sub><sup>8</sup>If this is inconvenient due to the PCB location in the model, you still have the option of disconnecting **both** power sources from the PCB.</sub><br/>
<sub><sup>9</sup> Less than 100µA. Powering off is achieved by the MCU setting both CTL1 and CTL2 lines to high so that all 4 MOSFET's will be closed and virtually no current will be drawn by the receiver and servos. The LED's are off. The STM32 MCU will enter STOP mode drawing only a few micro-amps. The only remaining currents are due to the quiescent currents of the LTC4412's (about 20µA total), HT7533 regulator (< 5µA), AH180 hall effect sensor (< 15µA) and the high value resistances associated with the voltage sensors (about 10µA total).</sub><br/>

## S.Port reporting

### Values

- MAIN PWR voltage
- STBY PWR voltage

These are transmitted using sensor ID: **TBD**

### Messages

| Message               | Condition                                                          |
| :-------------------: | :----------------------------------------------------------------- |
| USING MAIN PWR        | System configured for strategy #1 and MAIN PWR is the power source |
| USING STBY PWR        | System configured for strategy #1 and STBY PWR is the power source |
| STBY PWR LOW          | STBY PWR is a battery and it is below its minimum voltage          |
| MAIN PWR LOW          | MAIN PWR is a battery and it is below its minimum voltage          |
| MAIN PWR DISCONNECTED | MAIN PWR is below 0.5V                                             |
| STBY PWR DISCONNECTED | STBY PWR is below 0.5V                                             |
| CRITICAL              | Both MAIN PWR and STBY PWR are either LOW or DISCONNECTED          |

Note: these are non standard ad-hoc messages transmitted using sensor ID: **TBD**.
Adequate programming of the transmitter using OpenTX must be done to have audio and/or visual messages corresponding to each reported message.

If using a non-OpenTX transmitter, e.g., FrSky Tandem X20, an adequate audio and/or visual message must be configured in response to the MAIN PWR voltage value reporting.

## SBUS2 reporting

**TODO**

## LED signalling

| Condition <sup>*</sup>                                         | Green (LED1)  | Yellow (LED2) |
| -------------------------------------------------- | :----------:  | :-----------: |
| Power off                                          |      OFF      |      OFF      |
| Magnet detected                                    |       x       |     BRIGHT    |
| MAIN PWR LOW or DISCONNECTED                       |   BLINK FAST  |       x       |
| STBY PWR LOW or DISCONNECTED                       |       x       |   BLINK FAST  |
| Using STBY PWR (system configured for strategy #1) |       x       |   BLINK SLOW  |
| Using STBY PWR (system configured for strategy #2) |       x       |      DIM      |
| Using MAIN PWR                                     |      DIM      |       x       |

##  

<sub><sup>*</sup> Listed in order of priority. For example, if both MAIN PWR and STBY PWR are LOW or DISCONNECTED (CRITICAL condition), LED1 and LED2 will both blink fast whichever source is powering the model.</sub>

## System configuration

**TBD**

Using buttons or by programming through the S.Port connector:
