# Dual Microcontroller-Based Door Locker Security System Using Password Authentication

This project implements a smart, secure door control system that uses password authentication and dual microcontroller (MCU) communication for efficient functionality. Designed with advanced embedded systems techniques, this project leverages UART, EEPROM, motor control, a PIR sensor, and a keypad to enhance the security and usability of door access control.

---

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Operation Steps](#operation-steps)
- [System Requirements](#system-requirements)
- [Drivers and Libraries](#drivers-and-libraries)
- [References](#references)

---

## Project Overview
This door control system uses two ATmega32 MCUs:
- **HMI_ECU (Human-Machine Interface):** Manages user interactions, receiving password input through an LCD and keypad.
- **Control_ECU:** Processes password verification, controls door access with a motor, and handles additional functions like motion detection with a PIR sensor and security alerts with a buzzer.

The system enables secure password management, password-protected door unlocking, and includes advanced security mechanisms like timed locks and alarm activation after multiple failed attempts.

---

## Features
1. **Password Authentication:** Users can set and manage passwords stored securely in EEPROM.
2. **LCD and Keypad Interface:** User-friendly interface for entering and managing passwords.
3. **UART Communication:** Ensures smooth data exchange between HMI_ECU and Control_ECU.
4. **External EEPROM:** Stores passwords and system settings.
5. **Motorized Door Control:** Uses H-bridge for motorized door operation.
6. **Buzzer Alerts:** Activates on incorrect password attempts or security alerts.
7. **PIR Motion Detection:** Detects movement to control door state.
8. **Change Password Option:** Allows users to update the password after verification.
9. **Security Lock:** Locks system for one minute after three failed attempts.

---

## Hardware Components
### HMI_ECU Connections
1. **LCD (8-bit mode)**
   - RS → PC0
   - Enable → PC1
   - Data Pins D0-D7 → Port A (PA0 - PA7)

2. **Keypad (4x4)**
   - Rows → PB0 - PB3
   - Columns → PB4 - PB7

3. **UART Communication**
   - TXD and RXD pins connect to Control_ECU.

### Control_ECU Connections
1. **EEPROM (I2C Communication)**
   - SCL → PC0
   - SDA → PC1

2. **Buzzer**: Connected to PC7.
3. **H-bridge Motor Driver**
   - Input 1 → PD6
   - Input 2 → PD7
   - Enable1 → PB3 (OC0)

4. **Motor for Door Control**: Connects to the H-bridge.
5. **PIR Sensor**: Connects to PC2 for motion detection.

---

## Operation Steps

1. **Create System Password**
   - LCD prompts user to enter a 5-digit password and confirm it.
   - Password is verified and stored in EEPROM.

2. **Main Options Menu**
   - User can select between unlocking the door or changing the password.

3. **Open Door**
   - User enters the password to unlock the door.
   - If the password is correct, the motor rotates to unlock, and the PIR sensor detects motion to hold the door open.
   - Once clear, the door locks automatically.

4. **Change Password**
   - Password verification followed by a prompt to enter a new password.

5. **Security Lock Mechanism**
   - After three incorrect password attempts, buzzer activates for 1 minute and the system locks temporarily.

---

## System Requirements
- **System Frequency:** 8 MHz
- **Microcontroller:** ATmega32
- **Architecture:** Layered model with separate HMI_ECU and Control_ECU

---

## Drivers and Libraries

### Required Drivers
- **GPIO Driver**: Used across both ECUs.
- **UART Driver**: Custom UART driver for communication.
- **LCD and Keypad Drivers**: For HMI_ECU interface.
- **I2C Driver**: For EEPROM communication on Control_ECU.
- **PWM Driver**: Controls motor speed via Timer0.
- **Timer Driver**: Manages timing operations for motor control and message display.
- **Buzzer Driver**: Manages alerts and notifications.
- **PIR Driver**: Detects motion to hold door state.
- **DC Motor Driver**: Drives motor in coordination with Timer0 PWM.

### Configuration Structures
- **UART_ConfigType** and **TWI_ConfigType**: Allows flexible configuration of UART and I2C protocols.
- **Timer_ConfigType**: Manages timer operations across both ECUs.

---

## References
- **Video Demo**: [Dual Microcontroller Door Lock System](https://youtu.be/X5EwIRfGAAY)
- **PIR Sensor Library Setup**: [Proteus PIR Library Guide](https://youtu.be/bhT90bM-Vdw)
- **PIR Sensor Library Download**: [Download PIR Library](https://www.mediafire.com/file/vjffcf868ea2fbi/PIR_Sensor_lib.zip/file)
