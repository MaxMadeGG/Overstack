# Overstack

A tiny 30×30 mm pixel buddy with a 10×10 LED face that you can chat with an AI on. Tilt to scroll letters, press to pick, long press to send. The answer scrolls on the LEDs and it buzzes for feedback.

At first it had a touch screen, speaker and mic, but that was too big and too expensive, so I cut it down to the fun parts: motion, vibration and pixels.

## How It Works
Two stacked boards connected with 3 wires.

**Main board:** XIAO ESP32S3 (WiFi + AI), MPU6050 for tilt, one button, a vibration motor from an old Pixel (BC337 + diode), and a LiPo with a power switch. It charges over the XIAO's USB-C.

**LED board:** 100 WS2812B-2020 LEDs in a 10×10 grid on one data line, powered from the battery. It's 4 layers with GND and power planes, plus a few caps by the connector.

**Wiring**

| Main board | LED board |
| :--- | :--- |
| J_LED 1 (BAT_SW) | H1 1 |
| J_LED 2 (GND) | H1 2 |
| J_LED 6 (LED_DATA) | H1 3 |

Battery, switch and motor are soldered to pads on the main board. If the LED board fails, a 0.96" I²C OLED can plug into the spare J_LED pins (3V3, GND, SDA, SCL).

## Bill of Materials (BOM)
| Part | Description | Qty | Price | Vendor |
| :--- | :--- | :--- | :--- | :--- |
| XIAO ESP32S3 | Microcontroller | 1 | $0.00 | [Amazon.de]() |
| LiPo 402525 | 3.7 V 200 mAh battery | 1 | $0.00 | [Amazon.de]() |
| MPU6050 | Accelerometer + gyro | 1 | $0.00 | Local shop |
| Vibration motor | From an old Pixel | 1 | $0.00 | Salvaged |
| BC337, 1N4148, resistors, 100 nF cap | Motor driver + battery sense | 1 set | $0.00 | Local shop |
| Button + slide switch | Input + power | 1 each | $0.00 | Local shop |
| WS2812B-2020 | RGB LED (C52917434) | 100 | $0.00 | JLCPCB |
| 1 µF / 10 µF 0603 caps | LED board power | 3 | $0.00 | JLCPCB |
| 0.96" OLED SSD1306 (2-pack) | Backup screen | 1 | $8.00 | [Amazon.de]() |
| **Total** | | | **$0.00** | |

## Photos & Screenshots
* **Schematic:** Add screenshot here
* **PCB Layout:** Add screenshot here
* **3D Model / Build:** Add photo/render here

## Repository Structure
- `hardware/` — EasyEDA Pro files and Gerbers for both boards
- `firmware/` — Code for the board
- `cad/` — 3D files (`.step`) and printable models (`.stl`, `.3mf`)
