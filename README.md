# Color Based Robotic Arm 🤖

## 📌 Overview
This project is a color-based object sorting robotic arm. It automatically picks an object and places it in the correct position based on its detected color.

## 📸 Project Image
![Project](images/your_image.jpg)

## ⚙️ Components Used
- ESP32-WROOM-32
- 4 Servo Motors
- Color Sensor (TCS3200) / Serial Input
- External 5V Power Supply

## 🔧 Working Principle
1. Detects object color (Red, Blue, Green)
2. ESP32 processes the input
3. Robotic arm picks the object
4. Moves to predefined angle
5. Places object in corresponding box

## 🎯 Features
- Color-based sorting
- Smooth servo motion
- Pick and place operation
- Object counting system

## 🧠 Control System Concept
- Type: Open Loop Control System
- Position Control using Servo Motors

## 📦 Applications
- Industrial Automation
- Packaging Systems
- Warehouse Sorting
- Manufacturing

## 🔌 Connections
| Servo | GPIO |
|------|------|
| Base | 18 |
| Shoulder | 19 |
| Elbow | 21 |
| Gripper | 22 |

## 🚀 Future Improvements
- Add WiFi control
- IoT dashboard
- Camera-based detection using OpenCV

## 👨‍💻 Author
Parth Bujade
