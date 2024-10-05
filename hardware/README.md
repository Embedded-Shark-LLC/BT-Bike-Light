# Hardware

## Architecture diagram
![HW architecture diagram](/docs/DWG_HW-Architecture.svg)

## Major components
| Description           | Part Number           | Manufacturer          | Comments                          |
| --------------------- | --------------------- | --------------------- | --------------------------------- |
| Bluetooth module      | BT832                 | Fanstel               | 1.7-3.6 V, nRF52832               |
| USB C port            | UJC-HP2-3-SMT-TR      | CUI Devices           | Power only, horizontal            |
| Battery management IC | nPM1300               | Nordic Semiconductor  | 800 mA charge, onboard buck/LDO   |
| Interface button      | PTS645VM31-2 LFS      | C&K                   | Right angle, THT                  |
| LED driver            | TPS92201              | Texas Instruments     | 2.5-5.5 V, 1.5 A                  |
| Accelerometer         | LIS2DTW12             | STMicroelectronics    | 1.62-3.6 V, I2C                   |
| Red LED               | XPEBRD-L1-0000-00901  | Cree                  | 1.5 A, ~2.65 V @ 1.5 A            |
| Buzzer                | SMT-0827-S-HT-R       | PUI Audio             | 2-5 V, 2.7 kHz, 85 dB @ 3.6V      |
| Debug header          | FTSH-105-01-L-DV-K-P  | Samtec                | 10-pin SWD                        |

## Minor components
| Description           | Part Number           | Manufacturer          | Comments                          |
| --------------------- | --------------------- | --------------------- | --------------------------------- |
| Fuse                  | TR/0603FA5-R          | Eaton                 | 5A rating, 35A breaking capacity  |
| Buzzer NMOS FET       | DMN3731U-7            | Diodes Incorporated   | Vgs < 1.8 V                       |
| 10K NTC thermistor    | NCU15XH103F6SRC       | Murata Electronics    | Matches nPM1300 specs, 0402       |
| 2.2 uH inductor (nPM) | WLFM201209M2R2PC      | Walson Technology     | 170 mR DCR, 0805                  |
| 2.2 uH inductor (LED) | SDER041H-2R2MS        | Cyntek                | 3.1A, 45mR DCR                    |