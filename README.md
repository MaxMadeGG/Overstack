**Overstack**

Overstack was made for Hack Club Half-Life. Overstack is a tiny device that you can play games on it, talk to AI, use a calculator and more. Inside there is a seed Xiao ESP32-S3, MPU, vibration motor and LCD screen all stacked in a 30x30x15mm shell. You navigate through menus by tilting the device, using button to select and hold to send. Vibrates for feedback and shows answer on screen. The calculator or talking to the AI works by choosing a letter or number to make a word and then make a sentence, then hold the button to send and then know the answer or number.

**How It Works**:
Main board: XIAO ESP32S3 (WiFi + AI), MPU6050 for tilt, one button, a vibration motor and a LiPo with a power switch. It charges over the XIAO's USB-C.

**Screen**: Waveshare 128×128 LCD

**Wiring**

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

**Bill of Materials** (BOM)
| Part | Value | Qty | Price | Source | Have it? |
| :--- | :--- | :--- | :--- | :--- | :--- |
| XIAO ESP32S3 | Microcontroller | 1 | $17.00 | Amazon.de | No |
| Waveshare 0.85" LCD | 128×128 screen, SPI | 1 | $15.00 | Amazon.de | No |
| LiPo battery | 402525, 3.7 V 200 mAh | 1 | $13.00 | Amazon.de | No |
| MPU6050 (GY-521) | Accelerometer + gyro | 1 | $3.00 | Amazon.de | Yes |
| Vibration motor | Cylinder motor | 1 | $0.00 | Salvaged (Pixel) | Yes |
| BC337-40 | NPN transistor | 1 | $0.20 | Local shop | No |
| 1N4148 | Diode | 1 | $0.10 | Local shop | No |
| Resistor | 1 kΩ | 1 | $0.10 | Local shop | No |
| Resistor | 10 kΩ | 2 | $0.20 | Local shop | No |
| Capacitor | 100 nF | 1 | $0.10 | Local shop | Yes |
| Tactile button | Side push | 1 | $0.00 | Salvaged | Yes |
| Slide switch | Power on/off | 1 | $0.00 | Salvaged | Yes |
| Female header | 1×8, 2.54 mm | 1 | $0.40 | Local shop | Yes |
| Wire | Thin silicone wire | 1 | $1.00 | Amazon.de | Yes |
| Shipping | Amazon.de + buffer | 1 | $12.00 | Amazon.de | No |
| **Total (budget)** | | | **$62.10** | | |
| PCB | 2-layer, 30×30 mm, 5 pcs | 1 | $2.00 | JLCPCB | No |
| PCB shipping | Estimate | 1 | $4.00 | JLCPCB | No |
| **Total (PCB, not in budget)** | | | **$6.00** | | |

**Photos & Screenshots**
Schematic: <img width="1364" height="661" alt="image" src="https://github.com/user-attachments/assets/cb5ae8b8-fc70-4f4a-ae5f-44452a09bc34" />

**PCB Layout**: <img width="845" height="839" alt="image" src="https://github.com/user-attachments/assets/2786a0b4-ccbc-4134-8632-328596a2327d" />

**3D Model / Build**: <img width="1022" height="829" alt="image" src="https://github.com/user-attachments/assets/0769fe2d-6a93-4770-b64f-d6181b967a31" />




## Repository Structure
- `hardware/
- `firmware/
- `cad/
 > **Note:** The build and CAD is to be added 
