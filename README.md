# RX Dual Power

## Description

<img width="500px" src="Additional Documents/img/PCB Design Top.png">
<!-- <img width="500px" src="Additional Documents/img/PCB Design Bottom.png"> -->

Battery failures are one of the most common failures affecting radio controlled models. When this happens, the model becomes uncontrollable resulting most of the time in its loss or destruction. For electric models, the radio system is often powered by the BEC <sup><a href="#note1">1</a></sup> present on the ESC <sup><a href="#note2">2</a></sup>. In this case, another relatively common failure is the destruction of the ESC due to overload <sup><a href="#note3">3</a></sup> or other reasons, resulting in the associated BEC being destroyed as well.

This is a safety device for RC models allowing to power the radio system (receiver, servos, sensors, etc.) from either the normal electrical source or from a backup battery.

Commercial devices are available having this capability. Some are quite sophisticated but more bulky (and expensive!). Others are smaller and would flash a high power LED on the model when the backup battery is used but have no telemetry <sup><a href="#note3">3a</a></sup>.

The RX Dual Power:
* Can use any electrical power source used in radio controlled models both for primary and backup sources: BEC or battery (LiPo, LiFe, NiMh).
* Is able to use telemetry to communicate to the transmitter whether normal or backup power source is used. The transmitter can then be programmed to output vocal messages to reflect this. It will also transmit the voltage of both power sources.
* Uses a magnet to power off the receiver and servos (or buttons if you forgot your magnet !).
* Is small enough (60x40mm) the be placed in most models.

The RX Dual Power uses either S.Port or SBUS2 to communicate with the receiver and will only work with Futaba, FrSky or compatible receivers. A transmitter using OpenTX is ideal but not mandatory.

[Schematic](https://github.com/pvico/RX-Dual-Battery-Switch-R2.0/blob/main/Additional%20Documents/pdf/RX%20Dual%20Power%20Switch.pdf)

*The Gerber files of the PCB will be published here after testing the system in real use will be completed.*

## Types of power sources

The main power source (labelled MAIN PWR on the back of the PCB) and the standby power source (labelled STBY PWR on the back of the PCB) can be any of the following:

- BEC from 5 to 8.4V
- LIPO 2-4S (7.4 to 14.8V nominal, 8.4 to 16.8V fully charged)
- LIFE 2-4S (6.6 to 13.2V nominal, 7.2 to 14.4V fully charged)
- NIMH 4-5S (4.8 to 6V nominal, not recommended)

The maximum allowed voltage is 16.8V (fully charged LIPO 4S). The minimum source voltage is 5V. A NIMH 4S (4.8V) can be used but NIMH batteries are generally not recommended as they tend to be unreliable.

Any voltage present at the power source inputs can be present at the output. So, if **one or both** of the power source voltages is above the maximum voltage of the receiver or servos, *a BEC must be placed **after** the RX Dual Power and **both** power source voltages must be sufficient to drive that BEC*.

With the *available voltage* <sup><a href="#note4">4</a></sup> decreasing due to battery discharge, the receiver and servos will remain powered at all times until this voltage becomes so low that the receiver and/or servos fail.

**Note:** when using a BEC as input to MAIN PWR or STBY PWR, the RX Dual Power will measure the BEC's output voltage and will not detect a low battery voltage situation until the BEC fails due to low battery.

## Power source selection strategy

The system can be configured to use one of the following strategies for the selection of the power source:

- **Strategy #1**: Use MAIN PWR as long as it is available and has sufficient voltage, otherwise use STBY PWR
- **Strategy #2**: Use the power source with the highest voltage


### System configured for strategy #1

1. As long as MAIN PWR is above its minimum voltage (according to the type of source), MAIN PWR is selected and STBY PWR is isolated from the system. <sup><a href="#note5">5</a></sup>.

2. When MAIN PWR is *below its minimum voltage* <sup><a href="#note6">6</a></sup> - but **not** disconnected or in short-circuit - STBY PWR is selected and MAIN PWR is isolated from the system <sup><a href="#note7">7</a></sup>.

3. If MAIN PWR and STBY PWR are **both** below minimum voltage  - or if **any one** is disconnected or in short-circuit -  strategy #2 is applied.

### System configured for strategy #2

The source with the highest voltage powers the model, the other one is isolated from the system <sup><a href="#note8">8</a></sup>. 

Note: with strategy #2, if two batteries of the same type and number of cells are used as power sources, they will be selected alternatively <sup><a href="#note9">9</a></sup> and will discharge in parallel.

**For most situations, strategy #1 makes more sense**. Strategy #2 may be useful is some cases <sup><a href="#note9aa">9aa</a></sup>.

## Typical configurations

### Airplane or glider with electric motor

### Glider

A non-powered glider will use 2 batteries.

- If **all** servos are of "HV" type (high voltage) <sup><a href="#note9a">9a</a></sup>, you could use for example a 2S 1500mAh LiPo or 1600mAh LiFe ("18650" cells) as MAIN PWR and a 2S 500mAh LiPo or 300mAh LiFe ("CR2" cells) as STBY PWR.
- If **any** servo is not of HV type, you could use <sup><a href="#note9b">9b</a></sup>
    * Any 3-4S LiPo or LiFe battery for both MAIN PWR and STBY PWR and place a BEC after the RX Dual Power
    * A 3-6S LiPo or LiFe and a BEC as MAIN PWR and a 4S NiMh as STBY PWR
    * 4S NiMh batteries for both MAIN PWR and STBY PWR

### Airplane with gas/glow engine

Same as glider above. Size the batteries according to the model: using a 300mAh on a big gas engine model is not appropriate!

### Helicopter and multicopter

The RX Dual Power is probably not very useful for multi-copters. Should the main power source fail, the motor(s) will stop and the model will crash anyway.

However, an helicopter could still be landed in autorotaion after a failure of the main power source and the RX Dual Power can be used with a STBY battery just like an airplane with electric motor.

## Powering off the model

To power off the receiver and servos, use a neodymium magnet of sufficient size and power. To be detected, it must be positioned either *directly above* or *directly below* the hall effect sensor on the PCB *but not on its side*. The hall effect sensor location is clearly indicated on the PCB with the label 'MAGNET' and an arrow.

<img width="500px" src="Additional Documents/img/Magnet_on_PCB.jpg">

Approaching the magnet close to the hall effect sensor twice within 2 seconds will power off the model. When the model is powered off, approaching the magnet once will power the model back on.

Position the RX Dual Power PCB appropriately in the model to be able to power off/on without opening any canopy or cover. Using a stack of magnets allows to adjust the detection distance by adding or removing magnets <sup><a href="#note10">10</a></sup>.

Alternatively, if you don't have a magnet, press both buttons (SW1 and SW2) simultaneously for 2" to power off the model <sup><a href="#note11">11</a></sup>. Press any button to power it back on.

Connecting a power source, e.g. a new battery, will also power on the model.

When the model is powered off, the current consumed is minimal <sup><a href="#note11">12</a></sup>.

## S.Port reporting

### Values

- MAIN PWR voltage
- STBY PWR voltage

These are transmitted using sensor ID: **TBD**

### Messages

| Message               | Value | Condition                                                          |
| :-------------------: | :---: | :----------------------------------------------------------------- |
| USING MAIN PWR        |  TBD  | System configured for strategy #1 and MAIN PWR is the power source |
| USING STBY PWR        |  TBD  | System configured for strategy #1 and STBY PWR is the power source |
| STBY PWR LOW          |  TBD  | STBY PWR is a battery and it is below its minimum voltage          |
| MAIN PWR LOW          |  TBD  | MAIN PWR is a battery and it is below its minimum voltage          |
| MAIN PWR DISCONNECTED |  TBD  | MAIN PWR is below 0.5V                                             |
| STBY PWR DISCONNECTED |  TBD  | STBY PWR is below 0.5V                                             |
| CRITICAL              |  TBD  | Both MAIN PWR and STBY PWR are either LOW or DISCONNECTED          |

Note: these are non standard ad-hoc messages transmitted using sensor ID: **TBD**.
Adequate programming of the transmitter using OpenTX must be done to have audio and/or visual messages corresponding to each reported message.

If using a non-OpenTX transmitter, e.g., FrSky Tandem X20, an adequate audio and/or visual message must be configured in response to the MAIN PWR voltage value reporting.

## SBUS2 reporting

**TODO**

## LED signalling

| Condition <sup>13</sup>                                         | Green (LED1)  | Yellow (LED2) |
| -------------------------------------------------- | :----------:  | :-----------: |
| Power off                                          |      OFF      |      OFF      |
| Magnet detected                                    |       x       |     BRIGHT    |
| MAIN PWR LOW or DISCONNECTED                       |   BLINK FAST  |       x       |
| STBY PWR LOW or DISCONNECTED                       |       x       |   BLINK FAST  |
| Using STBY PWR (system configured for strategy #1) |       x       |   BLINK SLOW  |
| Using STBY PWR (system configured for strategy #2) |       x       |      DIM      |
| Using MAIN PWR                                     |      DIM      |       x       |

## System configuration

**TBD**

Using buttons or by programming through the S.Port connector:
##  

<sub><sup id="note1">1</sup> Battery Eliminator Circuit, a device reducing the battery voltage to a level acceptable by the radio system. Most of the time, the BEC is a buck converter: a switching mode step-down DC-DC converter.</sub><br/>
<sub><sup id="note2">2</sup> Electronic Speed Controller. Electric motors used today on RC models are usually of the brushless type. Brushless motors need a specific speed controller called an ESC and generally use LiPo (lithium polymer) batteries consisting of 3 cells or more in series (labelled 3S, 4S, etc.). Very often, ESC's are equipped with a BEC on the same PCB. When they are not, they are usually labelled "opto"</sub><br/>
<sub id="note3">3<sup></sup> Overloading the ESC can be due to motor/propeller mis-match, using an ESC of an insufficient current rating, using a battery with too many cells, etc. Sadly, many modellers have no real idea of what combination of motor, propeller and battery to use (although an excellent power drive calculator is available online here: [https://www.ecalc.ch](https://www.ecalc.ch)). Too often, I have heard modellers having a perfectly matched LiPo 3S power drive say "Oh, I will try this with a 4S to have more power". If have seen an ESC fail because the motor was inavertently controlled to run while the glider was on the ground and the propeller was blocked by contact with the ground.</sub><br/>
<sub id="note3a">3a<sup></sup>I have seen a schematic on the web of a system using the same LTC4412 circuits as this project but without the possibility to choose the power source selection strategy as explained below (it uses strategy #2) and with no telemetry.</sub><br/>
<sub><sup id="note4">4</sup> The available voltage is the highest of MAIN PWR voltage and STBY PWR voltage. The HT7533 voltage regulator delivering the 3.3V supply to the STM32 MCU is powered by this voltage less a diode drop. If the available voltage drops below 5V, the MCU VDD supply will gradually become unregulated but will initially remain at 3.3V. When the available voltage drops below about 4V, the MCU VDD will drop below 3.3V and the power source voltage measurements will become invalid. Anyway, well before this happens, the MCU will have switched to strategy #2 and whichever power source has the highest voltage will power the receiver and servos. When the available voltage drops below about 2.5V, the MCU will shut down and the LTC4412's behaviour is uncertain but most receivers and servos will have failed before reaching that voltage.</sub><br/>
<sub><sup id="note5">5</sup> By setting the CTL2 line to HIGH and releasing the CTL1 line (MCU pin set to high impedance).</sub><br/>
<sub><sup id="note6">6</sup> Below minimum voltage only applies to a battery. A BEC is never below minimum voltage: it is either above minimum voltage (> 4.8V) or considered disconnected (< 4.8V).</sub><br/>
<sub><sup id="note7">7</sup> The MCU releases the CTL2 line and sets the CTL1 line to HIGH, forcing MAIN PWR off. Note that we have to do this instead of applying strategy #2 because MAIN PWR below its minimum voltage can still be above the STBY PWR voltage: consider the case of a discharged LIPO 2S as MAIN PWR (< 7.2V) and a fully charged LIFE 2S (7V) or NIMH 4S (5.4V) as STBY PWR .</sub><br/>
<sub><sup id="note8">8</sup> The MCU lets the LTC4412's control the power source by releasing both CTL1 and CTL2 lines (MCU pins set to high impedance).</sub><br/>
<sub><sup id="note9">9</sup> The LTC4412's select whichever one is 20mV above the other.</sub><br/>
<sub><sup id="note9aa">9aa</sup> Using a 2S LiPo as MAIN PWR and a 2S LiFe as STBY PWR is **not** such a case: if you use strategy #2, the LiPo will discharge down to 7.2 or even 7V before the LiFe takes over. Then as the LiFe starts to discharge, it will alternate between the LiPo and the LiFe and they will discharge in parallel. This can bring the LiPo to a low voltage that can damage it.</sub><br/>
<sub><sup id="note9a">9a</sup> Most servos are not HV and have a maximum voltage of 6V, some as low as 5.5V. HV servos usually have a max voltage of 8.4V but some are limited to 7.4V. In this last case use a 2S LiFe instead of a 2S LiPo. Check the specifications of your servos !</sub><br/>
<sub><sup id="note9b">9b</sup> There are other possibilities. For example a 3-6S LiPo or LiFe and a BEC as MAIN PWR and a 3-6S LiPo or LiFe and a BEC as STBY PWR. If the BEC powering MAIN PWR is set for a higher voltage than STBY PWR's BEC, using strategy #2 is perfectly ok.</sub><br/>
<sub><sup id="note10">10</sup> A stack of 8 round magnets of size 12x5mm will be detected when it is about 3cm (1 1/4") from the sensor. Be very careful when adding magnets to the stack: the pull force is very strong and they break really easily.</sub><br/>
<sub><sup id="note11">11</sup> If this is inconvenient due to the PCB location in the model, you still have the option of disconnecting **both** power sources from the PCB.</sub><br/>
<sub><sup id="note12">12</sup> Less than 100µA. Powering off is achieved by the MCU setting both CTL1 and CTL2 lines to high so that all 4 MOSFET's will be closed and virtually no current will be drawn by the receiver and servos. The LED's are off. The STM32 MCU will enter STOP mode drawing only a few micro-amps. The only remaining currents are due to the quiescent currents of the LTC4412's (about 20µA total), HT7533 regulator (< 5µA), AH180 hall effect sensor (< 15µA) and the high value resistances associated with the voltage sensors (about 10µA total).</sub><br/>
<sub><sup id="note13">13</sup> Listed in order of priority. For example, if both MAIN PWR and STBY PWR are LOW or DISCONNECTED (CRITICAL condition), LED1 and LED2 will both blink fast whichever source is powering the model.</sub>