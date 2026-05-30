# Firmware – PIC18F4321  
**Scalar Reflectometer Educational Platform**

This firmware runs on the **PIC18F4321** microcontroller of the low-cost educational scalar reflectometer. It performs real-time acquisition of the forward and reverse envelope detector voltages, computes the magnitude of the reflection coefficient |S₁₁|, streams the results via TTL serial communication to a host PC, and provides a GPIO interface for future extensions (tunable matching networks, RF switches, etc.).

Compatible with the platform described in the paper:  
**“A Low-Cost Educational Reflectometer for Microwave Teaching”**  

## Pin Configuration (PIC18F4321)

| Pin       | Function               | Direction | Usage in this firmware                          | Educational purpose |
|-----------|------------------------|-----------|--------------------------------------------------|---------------------|
| **RA0**   | AN0                    | Input     | ADC Channel – Forward detector voltage          | Proportional to incident RF power |
| **RA1**   | AN1                    | Input     | ADC Channel – Reverse detector voltage          | Proportional to reflected RF power |
| **RA2–RA5**| AN2–AN5              | Input     | Reserved / available for additional sensors     | Future extensions |
| **RC6**   | TX (EUSART)            | Output    | TTL serial transmission (115200 baud)           | Data streaming to PC (|S₁₁|, voltages, status) |
| **RC7**   | RX (EUSART)            | Input     | TTL serial reception                            | Commands from PC |
| **RD0**   | SCLK (GPIO)            | Output    | GPIO clock line                                 | Control of external devices |
| **RD1**   | SDATA (GPIO)           | Output    | GPIO data line                                  | Control of external devices |
| **RD2–RD7**| GPIO                 | —         | Free general-purpose I/O pins                   | LEDs, buttons, displays, RF switches, etc. |
| **RB0–RB7**| GPIO / Interrupt     | —         | Not used in this version                        | Available for advanced projects |
| **VDD/VSS**| Power supply         | —         | 5 V (regulated from USB)                        | Full board power |

> Note from source code: `TRISD = 0xFC;` configures RD0 and RD1 as outputs for the MIPI interface, leaving the rest of PORTD available.

## Code Structure

- `main.c`          → Main loop (`while(1) { MotorADC(); }`)
- `adc.h / adc.c`   → ADC initialization and dual-channel reading
- `sio.h / nova_sio.h` → EUSART serial communication (TX/RX TTL)
- `PacketLogic.h`   → Packet formatting sent to the host PC
- `TTimer.h`        → Timer and high-priority interrupt management
- `MIPI.c`          → `initMIPI()` and `SendMIPIconfig()` functions

## Main Functionality

- Simultaneous sampling of both envelope detectors using the internal ADC
- Basic offset correction and single-point calibration
- Real-time computation of |S₁₁| magnitude
- Continuous streaming of results over the serial port (easy visualization with any terminal or Python/MATLAB scripts)
- GPIO control interface ready for extensions (Stage 10 of the laboratory sequence)

## How to Compile and Flash (for students)

1. Open the project in MPLAB X IDE with the XC8 compiler.
2. Build the project.
3. Program the PIC18F4321 using PICkit 4, SNAP, or the on-board USB bootloader.
4. Connect the reflectometer USB port to a PC — it appears as a standard COM port.

## Educational Notes for Students (aligned with Sections V and VI of the paper)

- Students can easily modify `MotorADC()` to add averaging, multi-point calibration, VSWR calculation, or return-loss conversion.
- Changing baud rate or adding new commands is straightforward thanks to the modular `nova_sio` library.

## License
MIT License – Feel free to modify and redistribute while preserving the original authorship.
