# Hardware

## Architecture diagram
![HW architecture diagram](/docs/DWG_HW-Architecture.svg)

## Major components
| Description           | Part Number           | Manufacturer          | Comments                          |
| --------------------- | --------------------- | --------------------- | --------------------------------- |
| Bluetooth module      | BC805M                | Fanstel               | 1.7-3.6 V, 10 GPIO                |
| USB C port            | UJC-HP2-3-SMT-TR      | CUI                   | Power only, right angle           |
| Battery charger       | MCP73834-FCI/MF       | Microchip             | 4.2 V, 1 A, 6 hr timeout          |
| Battery connector     | 54                    | Keystone Electronics  | One side per battery              |
| LDO                   | TPS7A0218             | Texas Instruments     | 1.8 V, 60 nA quiescent current    |
| Battery monitor       | STC3100               | STMicroelectronics    | I2C, V, I, charge percent         |
| Accelerometer         | LIS2DTW12TR           | STMicroelectronics    | I2C, 1.62-3.6 V                   |
| Button                | PTS645VK58-2 LFS      | C&K                   | Right angle                       |
| LED driver            | PAM2808               | Diodes Incorporated   | 2.5-6 V, 1.5 A                    |
| Red LED               | XPEBRD-L1-0000-00901  | Cree                  | 1.5 A, ~2.65 V @ 1.5 A            |
| Buzzer                | AT-1220-TT-3V-2-R     | PUI Audio             | 2-5 V, 2 kHz, 85 dB @ 3V          |
| Debug header          | FTSH-105-01-L-DV-K-P  | Samtec                | 10-pin SWD                        |

## Minor components
| Description           | Part Number           | Manufacturer          | Comments                          |
| --------------------- | --------------------- | --------------------- | --------------------------------- |
| Resettable fuse       | PTS18126V260          | Eaton                 | 2.6 A hold, 5 A trip, 6 V max     |
| 10K NTC thermistor    | ERT-J0EM103J          | Panasonic             | B25/50 = 3900K                    |
| Dual NMOS FET         | DMG4800LSD            | Diodes Incorporated   | 22 mOhm @ Vgs = 4.5 V             |
| Buzzer NMOS FET       | DMN3731U-7            | Diodes Incorporated   | Vgs < 1.8 V                       |