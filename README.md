# Praktikum Eingebettete Systeme — STM32F429

Embedded systems laboratory projects for the **STM32F429I-Discovery** board, developed as part of the university module *Praktikum Eingebettete Systeme* (Practical Embedded Systems).

## Overview

This repository contains a set of exercises and projects implementing drivers and applications for the STM32F429 microcontroller: GPIO, ADC, DMA, timers, 7-segment and LCD display, joystick input, environmental sensing, and closed-loop fan control with a PI controller.

**Authors:** Mahmoud Mahmoud, Judy Abou Rmeh  
**Target:** STM32F429I-DISC1  
**IDE:** STM32CubeIDE / Eclipse-based (`.cproject`, `.project`)

---

## Hardware

- **Board:** STM32F429I-Discovery
- **MCU:** STM32F429ZIT6 (Cortex-M4, 180 MHz)
- **Peripherals used:** GPIO, ADC (with DMA), timers (PWM, input capture), SPI (LCD), I2C (BME280), external 7-segment display, joystick, potentiometers, fan with tachometer

---

## Project Structure

```
├── 00_Introduction    # GPIO, LEDs, HAL basics
├── 01_ESD             # 7-segment display (multiplexing, countdown)
├── 02_Joystick        # Joystick + ESD interaction
├── 03_LCD             # TFT LCD (ILI9341), text and bargraph
├── 04_Potis           # Potentiometers (ADC), LCD bargraphs
├── 05_Potis_DMA       # ADC + DMA for potentiometers
├── 06_Blinky_Dot      # Dot LED blink speed via potentiometer (TIM1)
├── 07_Dimming_Dot     # Dot LED brightness via PWM (TIM1)
├── P1_Fan_Control     # Fan speed control: potentiometer → PI controller → PWM, RPM on LCD
├── P2_Weatherstation  # BME280 environmental sensor (temp, pressure, humidity) on LCD
├── modules/           # Shared drivers and utilities
│   ├── bme280/        # BME280 sensor driver
│   ├── dot/           # Dot LED (PWM / blinking)
│   ├── env_sensor/    # Environmental sensor abstraction
│   ├── esd/           # 7-segment display driver
│   ├── fan/           # Fan control (PWM + tachometer + PI controller)
│   ├── joystick/      # 5-way joystick (GPIO)
│   ├── lcd/           # ILI9341 TFT driver + GFX
│   ├── median/        # Median filter (e.g. for RPM)
│   ├── my_lcd/        # LCD helpers (bargraph, etc.)
│   ├── potis/         # Potentiometers (ADC, polling)
│   ├── potis_dma/     # Potentiometers (ADC + DMA)
│   ├── stopwatch/     # Stopwatch utility
│   └── utils/         # Delay, GPIO helpers
├── CMSIS/             # ARM CMSIS + STM32F4 device headers
└── HAL_Driver/        # STM32F4 HAL sources
```

Each numbered folder (e.g. `01_ESD`, `P1_Fan_Control`) is a separate **STM32CubeIDE project** with its own `main.c`. The `modules/` directory holds reusable drivers included by the projects.

---

## Labs & Projects Summary

| Project            | Description |
|--------------------|-------------|
| **00_Introduction** | GPIO setup, LED toggling with HAL and software delay. |
| **01_ESD**          | 4-digit 7-segment display: multiplexing, countdown (9→0 per digit, 9999→0). |
| **02_Joystick**     | Joystick (UP/DOWN/LEFT/RIGHT/PRESS) controls digit and position on ESD; PRESS triggers countdown. |
| **03_LCD**          | ILI9341 TFT: countdown and bargraph demo. |
| **04_Potis**        | Two potentiometers via ADC (polling); values and bargraphs on LCD. |
| **05_Potis_DMA**    | Same as 04 with ADC + DMA for non-blocking potentiometer reads. |
| **06_Blinky_Dot**   | Potentiometer sets dot-LED blink frequency (TIM1). |
| **07_Dimming_Dot**  | Potentiometer sets dot-LED brightness via TIM1 PWM. |
| **P1_Fan_Control**  | Potentiometer → target RPM; PI controller drives PWM; tachometer measures RPM; target/current RPM on LCD. |
| **P2_Weatherstation** | BME280: temperature, pressure, humidity read over I2C and displayed on LCD. |

---

## Technologies & Concepts

- **C** (C99), **CMSIS**, **STM32 HAL**
- **Peripherals:** GPIO, ADC (single-channel and multi-channel with DMA), timers (PWM, input capture), SPI, I2C
- **Concepts:** Interrupts, DMA, closed-loop control (PI), median filtering, multiplexed 7-segment display, TFT graphics

---

## How to Build and Flash

1. **IDE:** Open the workspace in **STM32CubeIDE** (or Eclipse with ARM GCC and STM32CubeMX project import).
2. **Select project:** In the Project Explorer, choose the project you want (e.g. `P1_Fan_Control`).
3. **Build:** Project → Build Project (or Ctrl+B).
4. **Flash:** Run → Debug or Run (ST-Link connected to the Discovery board).

Each project is self-contained; include paths and source links to `modules/`, `CMSIS/`, and `HAL_Driver/` are set in the `.cproject` files.

---

## License

This project was developed for educational purposes as part of a university course. STM32 CMSIS and HAL components are subject to their respective licenses (see `CMSIS/` and `HAL_Driver/`).
