# bobbycar-boardcomputer-firmware

## Build status

| Service | Status |
| :---                |                                                                                                                   ---: |
| Actions (CI) | [![CI](https://github.com/bobbycar-graz/bobbycar-boardcomputer-firmware/actions/workflows/workflow.yml/badge.svg)](https://github.com/bobbycar-graz/bobbycar-boardcomputer-firmware/actions/workflows/workflow.yml) |
| Sonar | [![Bugs](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=bugs)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=duplicated_lines_density)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=security_rating)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Technical Debt](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=sqale_index)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware)<br />[![Vulnerabilities](https://sonarcloud.io/api/project_badges/measure?project=bobbycar-graz_bobbycar-boardcomputer-firmware&metric=vulnerabilities)](https://sonarcloud.io/summary/new_code?id=bobbycar-graz_bobbycar-boardcomputer-firmware) |
## How to clone ? (READ THIS OR YOU WILL FAIL!)

```bash
git clone --recursive git@github.com:bobbycar-graz/bobbycar-boardcomputer-firmware.git
cd bobbycar-boardcomputer-firmware/
./esp-idf/install.sh
./switchconf.sh feedc0de
```

## How to compile
Also do the initialization if you use an IDE, otherwise build will fail.

```bash
# before you try to build anything, always do this first:
. export.sh

# if needed, switch to the right configuration for your hardware (run with --list to list available configurations)
./switchconf.sh feedc0de

# flash bootloader, partition table and app (only do once)
idf.py -p /dev/ttyUSB0 -b 921600 flash monitor

# flash only app (do this for development as it is much faster)
idf.py -p /dev/ttyUSB0 -b 921600 app-flash monitor

# alternative commands (After '. export.sh')
bobby-build # Builds the firmware
bobby-flash # Flashes the firmware
bobby-app-flash # Just flashes the app partition
bobby-monitor # Opens the serial monitor
bobby-coredump # Opens the coredump-utility
open-ide # Opens qtcreator
```

If you are inside monitor, hit Ctrl+T then Ctrl+X to exit.

Hit Ctrl+T then Ctrl+A to reflash the app and return to monitor (very handy during development)

## How to wire all connections?

![Wiring diagram](/img/wiring.png)

### -DFEATURE_DPAD
![dpad switches with 5 wires](/img/dpadsw.png)

### -DFEATURE_DPAD_3WIRESW
![3 wire switches](/img/3wiresw.png)

### -DFEATURE_ROTARY
![rotary encoder](/img/rotary.png)
