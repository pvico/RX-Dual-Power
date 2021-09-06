# RX Dual Power

## Description

<p align="center"><a href="https://github.com/pvico/RX-Dual-Power/blob/main/Additional%20Documents/img/7200x4900.jpg"><img css="margin:0 auto;" width="600px" src="Additional Documents/img/RX Dual Power Top.jpg"></a></p>
<p align="center"><sup>Click on the picture for a very high resolution image (7200x4900)</sup></p>


The RX Dual Power is a safety device for RC models allowing to power the radio system (receiver, servos, sensors, etc.) from either the normal electrical source or from a backup battery.

Battery failures are one of the most common failures affecting radio controlled models. When this happens, the model becomes uncontrollable resulting most of the time in its loss or destruction. For electric models, the radio system is often powered by the BEC<sup><a href="#note1">1</a></sup> present on the ESC<sup><a href="#note2">2</a></sup>. In this case, another relatively common failure is the destruction of the ESC due to overload<sup><a href="#note3">3</a></sup> or other reasons, resulting in the associated BEC being destroyed as well.

<!-- Commercial devices with this capability are available. Some are quite sophisticated but more bulky (and expensive!). Others are smaller and would flash a high power LED on the model when the backup battery is used but have no telemetry<sup><a href="#note4">4</a></sup>. -->

The RX Dual Power:
* Can use any electrical power source used in RC models both for normal and backup sources: BEC or battery (LiPo, LiFe, NiMh).
* Uses telemetry to communicate to the transmitter whether normal or backup power source is used. The transmitter can then be programmed to output vocal messages to reflect this. It will also transmit the voltage of both power sources.
* Uses a magnet to power off the receiver and servos (you can use the buttons if you forgot your magnet !).
* Is small enough (60x40mm) to be placed in most models.
* Can power the receiver and servos up to 10A. 
* XT30 or XT60 connectors can be soldered directly on the printed circuit board (PCB) to connect batteries.

The RX Dual Power uses either S.Port or SBUS2 to communicate with the receiver and will only work with Futaba, FrSky or compatible receivers. A transmitter using OpenTX is ideal but not mandatory.
#

<sub><sup id="note1">1</sup> Battery Eliminator Circuit, a device reducing the battery voltage to a level acceptable by the radio system. Most of the time, the BEC is a buck converter: a switching mode step-down DC-DC converter.</sub><br/>
<sub><sup id="note2">2</sup> Electronic Speed Controller. Electric motors used today on RC models are usually of the brushless type. Brushless motors need a specific speed controller called an ESC and generally use LiPo (lithium polymer) batteries consisting of 3 cells or more in series (labelled 3S, 4S, etc.). Very often, ESC's are equipped with a BEC on the same PCB. When they are not, they are usually labelled "opto"</sub><br/>
<sub id="note3">3<sup></sup> Overloading the ESC can be due to motor/propeller mis-match, using an ESC of an insufficient current rating, using a battery with too many cells, etc. Sadly, many modellers have no real idea of what combination of motor, propeller and battery to use (although an excellent power drive calculator is available online here: [https://www.ecalc.ch](https://www.ecalc.ch)). Too often, I have heard modellers having a perfectly matched LiPo 3S power drive say "Oh, I will try this with a 4S to have more power". If have seen an ESC fail because the motor was inavertently controlled to run while the glider was on the ground and the propeller was blocked by contact with the ground.</sub><br/>
<!-- <sub id="note4"> 4<sup></sup> I have seen a schematic on the web of a device using the same LTC4412 circuits as this project but it always uses the source with the highest voltage and has no telemetry.</sub><br/> -->

## Power source selection

The normal primary power source is labelled MAIN PWR on the back of the PCB and the backup power source is labelled STBY PWR on the back of the PCB.

1. As long as MAIN PWR is above its minimum voltage (according to the type of power source), MAIN PWR is selected and STBY PWR is isolated from the system.

2. When MAIN PWR is *below its minimum voltage*<sup><a href="#note4">4</a></sup> - but **not** disconnected or in short-circuit - STBY PWR is selected and MAIN PWR is isolated from the system.

3. If MAIN PWR and STBY PWR are **both** below minimum voltage  - or if **any one** is disconnected or in short-circuit -  The source with the highest voltage powers the model, the other one is isolated from the system. The *available voltage* - the highest of MAIN PWR voltage and STBY PWR voltage - will further decrease due to the discharging batteries. However, the receiver and servos will remain powered until this voltage becomes so low that the receiver and/or servos fail.
#

<sub><sup id="note4">4</sup> Below minimum voltage only applies to a battery. A BEC is never below minimum voltage: it is either above minimum voltage (> 4.8V) or considered disconnected (< 4.8V).</sub><br/>

## LED signalling

| Condition <sup>*</sup>       | Green (LED1)  | Yellow (LED2) |
| ---------------------------- | :----------:  | :-----------: |
| Power off                    |      OFF      |      OFF      |
| Magnet detected              |       x       |     BRIGHT    |
| MAIN PWR LOW or DISCONNECTED |   BLINK FAST  |       x       |
| STBY PWR LOW or DISCONNECTED |       x       |   BLINK FAST  |
| Using STBY PWR               |       x       |   BLINK SLOW  |
| Using MAIN PWR               |      DIM      |       x       |

An external LED can be connected to the PCB. It will illuminate toghether with the yellow led to indicate when the magnet is detected or if STBY PWR is being used.

<sub><sup>*</sup> Listed in order of priority. For example, if both MAIN PWR and STBY PWR are LOW or DISCONNECTED (CRITICAL condition), LED1 and LED2 will both blink fast whichever source is powering the model.</sub>

## Powering off the model

To power off the receiver and servos, use a neodymium magnet of sufficient size and power. To be detected, it must be positioned either *directly above* or *directly below* the hall effect sensor on the PCB *but not on its side*. The hall effect sensor location is clearly indicated on the PCB with the label 'MAGNET' and an arrow.

<img width="500px" src="Additional Documents/img/Magnet_on_PCB.jpg">

Approaching the magnet close to the hall effect sensor twice within 2 seconds will power off the model. When the model is powered off, approaching the magnet once will power the model back on.

Position the RX Dual Power PCB appropriately in the model to be able to power off/on without opening any canopy or cover. Using a stack of magnets allows to adjust the detection distance by adding or removing magnets<sup><a href="#note5">5</a></sup>.

Alternatively, if you don't have a magnet, press both buttons (SW1 and SW2) simultaneously for 2" to power off the model <sup><a href="#note6">6</a></sup>. Press any button to power it back on.

When the model is powered off, the current consumed is minimal (60 *micro* amperes!). A small 300mAh battery would completely discharge in about 2 weeks<sup><a href="#note7">7</a></sup>. Nevertheless, it is best to always start a flight session with fully charged batteries.
#

<sub><sup id="note5">5</sup> A stack of 8 round magnets of size 12x5mm will be detected when it is about 3cm (1 1/4") from the sensor. Be very careful when adding magnets to the stack: the pull force is very strong and they break really easily.</sub><br/>
<sub><sup id="note6">6</sup> If this is inconvenient due to the PCB location in the model, you still have the option of disconnecting **both** power sources from the PCB.</sub><br/>
<sub><sup id="note7">7</sup> When powered off, the very small remaining current will be drawn from the highest voltage source. As an example, for an electric model using the ESC's BEC and with the BEC voltage lower than the backup battery, the RX Dual Power will draw this current from the STBY PWR battery. The main power drive battery, if remaining connected, will only provide the *quiescent current* of the ESC and its associated BEC (probably a few micro-amps as well). Even if the STBY PWR battery is very small (300mAh) and it provides the power off current, it should last the whole flight session and probably the following day as well, *provided there was never a condition where it was used in flight*</sub>

## S.Port reporting

### Values

- MAIN PWR voltage
- STBY PWR voltage

These are transmitted using sensor ID: **1 to 28, TBD**

### Messages

| Message               | Value | Condition                                                 |
| :-------------------: | :---: | :-------------------------------------------------------- |
| USING MAIN PWR        |  TBD  | MAIN PWR is the power source                              |
| USING STBY PWR        |  TBD  | STBY PWR is the power source                              |
| STBY PWR LOW          |  TBD  | STBY PWR is a battery and it is below its minimum voltage |
| MAIN PWR LOW          |  TBD  | MAIN PWR is a battery and it is below its minimum voltage |
| MAIN PWR DISCONNECTED |  TBD  | MAIN PWR is below 0.5V                                    |
| STBY PWR DISCONNECTED |  TBD  | STBY PWR is below 0.5V                                    |
| CRITICAL              |  TBD  | Both MAIN PWR and STBY PWR are either LOW or DISCONNECTED |

Note: these are non standard ad-hoc messages transmitted using sensor ID: **1 to 28, TBD**.
Adequate programming of the transmitter using OpenTX must be done to have audio and/or visual messages corresponding to each reported message.

If using a non-OpenTX transmitter, e.g., FrSky Tandem X20, an adequate audio and/or visual message must be configured in response to the MAIN PWR voltage value reporting.

## SBUS2 reporting

**TODO**

## Types of power sources

MAIN PWR and STBY PWR can be any of the following:

- BEC from 5 to 8.4V
- LIPO 2-4S (7.4 to 14.8V nominal, 8.4 to 16.8V fully charged)
- LIFE 2-4S (6.6 to 13.2V nominal, 7.2 to 14.4V fully charged)
- NIMH 4-5S (4.8 to 6V nominal, not recommended)

The maximum allowed voltage is 16.8V (fully charged LIPO 4S). The minimum source voltage is 5V. A NIMH 4S (4.8V) can be used but NIMH batteries are generally not recommended as they tend to be unreliable.

Any voltage present at the power source inputs can be present at the output. So, if **one or both** of the power source voltages is above the maximum voltage of the receiver or servos, *a BEC must be placed **after** the RX Dual Power and **both** power source voltages must be sufficient to drive that BEC*.

**Note:** when using a BEC as input to MAIN PWR or STBY PWR, the RX Dual Power will measure the BEC's output voltage and will not detect a low battery voltage situation until the BEC fails due to low battery.

## Typical configurations

### Glider

A non-powered glider will use 2 batteries.

- If **all** servos are of "HV" type (high voltage)<sup><a href="#note8">8</a></sup>, you could use for example a 2S 1500mAh LiPo or 1600mAh LiFe ("18650" cells) as MAIN PWR and a 2S 500mAh LiPo or 300mAh LiFe ("CR2" cells) as STBY PWR.
- If **any** servo is not of HV type, you could use<sup><a href="#note9">9</a></sup>
    * Any 3-4S LiPo or LiFe battery for both MAIN PWR and STBY PWR and place a BEC after the RX Dual Power
    * A 3-6S LiPo or LiFe and a BEC as MAIN PWR and a 4S NiMh as STBY PWR
    * 4S NiMh batteries for both MAIN PWR and STBY PWR

### Airplane or glider with electric motor

Most of the time, you will use the BEC associated with the ESC as MAIN PWR. 

For STBY PWR, use

- Any 3-6S LiPo or LiFe and a separate BEC
- All servos are HV: a 2S LiPo or LiFe
- If any servo is not HV: a 4S NiMh

If you use an "opto" ESC (it has no BEC), or you don't want to use the ESC's BEC (maybe because it has an insufficient current rating), then same as for glider above.

### Airplane with gas/glow engine

Same as glider above. Size the batteries according to the model: using a 300mAh on a big gas engine model is not enough!

### Helicopter and multicopter

For a glow engine helicopter: as for glow engine airplane.

An electric helicopter could still be landed in autorotaion after a failure of the main power source and the RX Dual Power can be used with a STBY battery just like an airplane with electric motor.

The RX Dual Power is probably not very useful for multi-copters. Should the main power source fail, the motor(s) will stop and the model will crash anyway.
#

<sub><sup id="note8">8</sup> Most servos are not HV and have a maximum voltage of 6V, some as low as 5.5V. HV servos usually have a max voltage of 8.4V but some are limited to 7.2 or 7.4V. In this last case use a 2S LiFe instead of a 2S LiPo. Check the specifications of your servos !</sub><br/>
<sub><sup id="note9">9</sup> There are other possibilities. For example a 3-6S LiPo or LiFe and a BEC as MAIN PWR and a 3-6S LiPo or LiFe and a BEC as STBY PWR.</sub><br/>

## Configuring the RX Dual Power

Using buttons or by programming through the S.Port connector: **TBD**

## PCB

*The Gerber files of the PCB will be published here after testing in real conditions has been completed.*

It is a 4 layer PCB with generous copper fill areas and plenty of vias to connect the layers. 

## Electronic circuit

<a href="(https://github.com/pvico/RX-Dual-Battery-Switch-R2.0/blob/main/Additional%20Documents/pdf/RX%20Dual%20Power%20Switch.pdf"><img css="margin:0 auto;" width="100%" src="Additional Documents/img/Schematic.png"></a>
<p align="center"><sup>Click on the picture for the pdf file</sup></p></br>

On each input, two P-channel MOSFET's (Q1/Q2 and Q3/Q4), mounted back-to-back<sup><a href="#note10">10</a></sup>, switch the associated power source on or off.

This circuit, using a STM32L021 micro-controller ("MCU" hereafter), is based on the Linear Technology LTC4412 "ideal diode" IC. This IC will drive the MOSFET's and will never allow a reverse current to enter the connected battery when the voltage at the output is higher than the battery, even if the MCU attempts to open the MOSFET's on both inputs.

By driving the CTL pin high, the MCU forces the LTC4412 to swicth its associated MOSFET's off. If the MCU drives the CTL pin low - or put its corresponding GPIO pin to high impedance - the LTC4412 will open its associated MOSFET's but only if its source voltage is at least 20mV above the output voltage (which could be at a higher voltage if powered by the other source). 

So, refering to the "Power source selection" section here above:
* Step 1: the MCU drives the CTL2 signal high, forcing U3 to swicth off Q3 and Q4. This isolates the STBY PWR source from the output. It puts its CTL1 pin to high impedance. U2 will drive its CTL pin to low, opening Q1 and Q2, connecting MAIN PWR to the output.
* Step 2: the MCU drives the CTL1 signal high, forcing U2 to swicth off Q1 and Q2. This isolates the MAIN PWR source from the output. It puts its CTL2 pin to high impedance. U3 will drive its CTL pin to low, opening Q3 and Q4, connecting STBY PWR to the output<sup><a href="#note11">11</a></sup>.
* Step 3 (when both sources are below minimum voltage or if any one is disconnected or in short-circuit): the MCU will put both CTL1 and CTL2 pins to high impedance. U2 and U3 will manage the source selection, connecting whichever source is 20mV above the other to the output.

The STAT_STBY signals to the MCU when STBY PWR is powering the output.

The input source voltages are measured by the MCU through R8/R15 and R9/R16.

The AH180 hall effect sensor signals to the MCU when a magnet is in close range to the sensor (MAGNET signal is driven low).

The HT7533 3.3V linear regulator will provide the 3.3V supplying the MCU, the Hall effect sensor and the LED's. It is powered by any of the two input voltages through the diodes D1 & D2. So the highest voltage input powers the HT7533. If the available voltage drops below 5V, the MCU VDD supply will gradually become unregulated but will initially remain at 3.3V. When the available voltage drops below about 4V, the MCU VDD will drop below 3.3V and the power source voltage measurements will become invalid. Anyway, well before this happens, the MCU will have put CTL1 and CTL2 to high impedance state and the LTC4412's will select whichever power source has the highest voltage to power the receiver and servos. When the available voltage drops below about 2.5V, the MCU will shut down and the LTC4412's behaviour becomes uncertain but most receivers and servos will have failed before reaching that voltage.

Powering off is achieved by the MCU setting both CTL1 and CTL2 lines to high so that all 4 MOSFET's will be closed and virtually no current will be drawn by the receiver and servos. The LED's are off. The STM32L021 will enter STOP mode drawing only a few micro-amps. The only remaining currents are due to the quiescent currents of the LTC4412's (about 20µA total), HT7533 regulator (< 5µA), AH180 hall effect sensor (< 15µA) and the high value resistances associated with the voltage sensors (about 10µA total).

When the model is powered off, if the AH180 detects a magnet or if a button is depressed, the SW1, SW2 or MAGNET signals will trigger an interrupt that will awake the MCU from the STOP mode. The MCU will then simply perform a restart, like if a power source was first connected to the PCB.

The configuration of the RX Dual Power is stored in the EEPROM of the STM32L021.
##  

<sub><sup id="note10">10</sup> This is required: if a single MOSFET was used, its body diode would allow a reverse current to flow into the connected battery when the output voltage coming from the other input source is higher than the battery voltage.</sub><br/>
<sub><sup id="note11">11</sup> Note that we have to do this instead of putting both CTL pins to low or high impedance because MAIN PWR below its minimum voltage can still be above the STBY PWR voltage: consider the case of a discharged LIPO 2S as MAIN PWR (< 7.2V) and a fully charged LIFE 2S (7V) or NIMH 4S (5.4V) as STBY PWR .</sub><br/>

## Building and Flashing the firmware

**TODO**

To connect your STM32 SWD programmer (e.g. STMicroelectronics ST-Link/V2) to the PCB, use a "DYKB 1.27mm spacing Test stand PCB clip, double row, 1.27mm-3P", available on Aliexpress.

<img css="margin:0 auto;" width="600px" src="Additional Documents/img/SWD Clip.jpg">

#

&nbsp;