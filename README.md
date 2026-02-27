# High-Frequency-Audio-Data-Acquisition-using-ESP32-WROOM-32

## Introduction

Design and implementation of a high-frequency audio data acquisition system using the ESP32-WROOM-32 microcontroller. The project aims to capture and process audio signals in real-time, leveraging the capabilities of the ESP32 for efficient data handling and transmission for audio tasks such as recording and playback.

---

## Structure

- `docs/`: Documentation and design notes.
  - `notes/`: Development notes and considerations.
  - `references/`: Reference materials and datasheets.
- `project-architecture/`: Overview of the project architecture, containing main driver code structure.
- `project-kicadspace/`: KiCad design files for the PCB layout and schematic.
- `project-obsidianspace/`: Obsidian vault for project notes and documentation.

---

## Overview

This project focuses on the development of a high-frequency audio data acquisition system using the ESP32-WROOM-32 microcontroller. The system is designed to capture audio signals at high sampling rates, process the data in real-time, and transmit it for further analysis or storage. The project encompasses both hardware design, including PCB layout and component selection, and software development for efficient data handling and communication. Key features include:

- Real-time audio data acquisition and processing.
- Efficient data transmission using the ESP32's Wi-Fi, Bluetooth & external storage capabilities.
- Robust firmware design for reliable operation in various conditions.

---

## Features

- **Microcontroller**: ESP32-WROOM-32 (ESP32-WROOM-32)
- **Peripherals**:
  - SPI for communication with external microSD card for data storage
  - I2S for audio data acquisition from the ADC
  - GPIO for status indicators and control signals
  - RCC for reset & clock management
  - IWDG for watchdog functionality
  - I2C for LCD interfacing (optional)
  - DMA for efficient data transfer from ADC to memory

---

## Hardware Components

Note that this sections can be expand with new requirements in the future.

- ESP32-WROOM-32 microcontroller
- MAX9812 microphone amplifier
- PCM1808 ADC
- TL072IDT operational amplifier
- MicroSD card module for data storage
- Optional: I2C LCD display for status monitoring
