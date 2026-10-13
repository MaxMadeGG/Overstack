Overstack

Overstack was made for Hack Club Half-Life. Overstack is a tiny device that you can play games on it, talk to AI, use a calculator and more. Inside there is a seed Xiao ESP32-S3, MPU, vibration motor and LCD screen all stacked in a 30x30x15mm shell. You navigate through menus by tilting the device, using button to select and hold to send. Vibrates for feedback and shows answer on screen. The calculator or talking to the AI works by choosing a letter or number to make a word and then make a sentence, then hold the button to send and then know the answer or number.

**How It Works**:
Main board: XIAO ESP32S3 (WiFi + AI), MPU6050 for tilt, one button, a vibration motor and a LiPo with a power switch. It charges over the XIAO's USB-C.

Screen: Waveshare 128×128 LCD

Wiring

| Screen | Main board |
| :--- | :--- |
| VCC | J_LED 3V3 |
| GND | J_LED GND |
| DIN | J_LED LED_DATA (D10) |
| DC | J_LED LED_EN (D3) |
| CLK | H3 HDR_GPIO3 (D8) |
| CS | GND |
| RST | 3V3 |
| BL | 3V3 |

Battery, switch and motor are soldered to pads on the main board.

Bill of Materials (BOM)
| Part | Description | Qty | Price | Vendor |
| :--- | :--- | :--- | :--- | :--- |
| XIAO ESP32S3 | Microcontroller | 1 | $17.00 | [Amazon.de]() |
| LiPo 402525 | 3.7 V 200 mAh battery | 1 | $13.00 | [Amazon.de]() |
| MPU6050 | Accelerometer + gyro | 1 | $0.00 | Local shop |
| Vibration motor | From an old Pixel | 1 | $0.00 | Salvaged |
| BC337, 1N4148, resistors, 100 nF cap | Motor driver + battery sense | 1 set | $0.00 | Local shop |
| Button + slide switch | Input + power | 1 each | $0.00 | Local shop |
| Waveshare 0.85" LCD | 128×128 IPS, SPI | 1 | $15.00 | [Amazon.de]() |
| **Total** | | | **$53.00** | |

Photos & Screenshots
Schematic: <img width="1364" height="661" alt="image" src="https://github.com/user-attachments/assets/cb5ae8b8-fc70-4f4a-ae5f-44452a09bc34" />

PCB Layout: <img width="845" height="839" alt="image" src="https://github.com/user-attachments/assets/2786a0b4-ccbc-4134-8632-328596a2327d" />

3D Model / Build: <img width="1302" height="903" alt="image" src="https://github.com/user-attachments/assets/7e9544f8-55e4-4a96-8cda-bc765b653f0f" />

## Repository Structure
- `hardware/
- `firmware/
- `cad/
 > **Note:** The build and CAD is to be added 
