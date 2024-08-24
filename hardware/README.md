# Hardware

## Architecture diagram
![HW architecture diagram](/docs/BT-Bike-Light_HW-Architecture.svg)

## List of components
- Bluetooth module
  - Part number: BC805M
  - Manufacturer: Fanstel
  - Operating voltage: 1.7-3.6 V
  - 10 GPIO pins
- Red LED
  - Part number: XPEBRD-L1-0000-00901
  - Manufacturer: CreeLED, Inc.
  - Max current: 1.5 A
  - Forward voltage @ 1.5A: ~2.65 V
- LED driver
  - Part number: PAM2808
  - Manufacturer: Diodes Incorporated
  - Max current: 1.5 A
  - Supply voltage: 2.5-6.0 V
- Accelerometer
  - Part number: LIS2DTW12TR
  - Manufacturer: STMicroelectronics
  - Operating voltage: 1.62-3.6 V
- USB-C port
- Buzzer
- Battery connector
- Battery charger
- Buck/boost converter (?)
  - Need to determine brightness of LED as battery dies using only LED driver
- Button
- Debug header
  - Part number: FTSH-105-01-L-DV-K-P
  - Manufacturer: Samtec Inc.
  - 10-pin SWD