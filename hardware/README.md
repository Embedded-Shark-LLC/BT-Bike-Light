# Hardware

## Architecture diagram
![HW architecture diagram](/docs/DWG_HW-Architecture.svg)

## Major components
| Description           | Part Number           | Manufacturer          | Comments                          |
| --------------------- | --------------------- | --------------------- | --------------------------------- |
| Bluetooth module      | BT832                 | Fanstel               | 1.7-3.6 V, nRF52832               |
| USB C port            | 2171780001            | Molex                 | Power only, vertical              |
| Battery management IC | nPM1300               | Nordic Semiconductor  | 800 mA charge, onboard buck/LDO   |
| Battery connector     | S2B-PH-K-S            | JST                   | Right angle                       |
| Battery status LED    | B38G3RGB              | Harvatek              | RGB                               |
| Button                | PTS645VK58-2 LFS      | C&K                   | Right angle                       |
| LED driver            | PAM2808               | Diodes Incorporated   | 2.5-6 V, 1.5 A                    |
| Red LED               | XPEBRD-L1-0000-00901  | Cree                  | 1.5 A, ~2.65 V @ 1.5 A            |
| Buzzer                | SMT-0827-S-HT-R       | PUI Audio             | 2-5 V, 2.7 kHz, 85 dB @ 3.6V      |
| Debug header          | FTSH-105-01-L-DV-K-P  | Samtec                | 10-pin SWD                        |

## Minor components
| Description           | Part Number           | Manufacturer          | Comments                          |
| --------------------- | --------------------- | --------------------- | --------------------------------- |
| Resettable fuse       | PTS18126V260          | Eaton                 | 2.6 A hold, 5 A trip, 6 V max     |
| Buzzer NMOS FET       | DMN3731U-7            | Diodes Incorporated   | Vgs < 1.8 V                       |
| 10K NTC thermistor    | NCU15XH103F6SRC       | Murata Electronics    | Matches nPM1300 specs, 0402       |
| 2.2 uH inductor       | WLFM201209M2R2PC      | Walson Technology     | 170 mR DCR, 0805                  |