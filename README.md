# Completed Lock and Light System

This project is an Arduino-based smart locker system designed for a delivery robot. When an order is placed (button press), a unique 4-digit code is generated and displayed. The user enters this code on a keypad to unlock the compartment.

## Features
- Random 4-digit order code generation
- Keypad input system
- Servo-controlled locking mechanism
- LCD display feedback
- NeoPixel LED status indicator
- Secure compartment unlocking

## Hardware Used
- Arduino Uno
- I2C LCD (0x27)
- 4x4 Keypad
- Servo Motor
- Push Button
- NeoPixel LED Strip

## Pin Configuration
- Servo: Pin 11  
- Button: Pin 10  
- NeoPixel: Pin 12  
- Keypad Rows: 9, 8, 7, 6  
- Keypad Columns: 5, 4, 3, 2  

## How It Works
1. Press button → generates order code  
2. Code displayed on LCD  
3. User enters code via keypad  
4. Correct code → servo unlocks + LED turns on  
5. After delay → system resets and locks  
