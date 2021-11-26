# bobbycar-boardcomputer-firmware

## Build status

| Service | Status |
| :---                |                                                                                                                   ---: |
| Actions (master) | ![Build Status](https://github.com/bobbycar-graz/bobbycar-boardcomputer-firmware/actions/workflows/main.yml/badge.svg) |
| Actions (cloud)  | ![Build Status (cloud)](https://github.com/bobbycar-graz/bobbycar-boardcomputer-firmware/actions/workflows/main.yml/badge.svg?branch=cloud) |

## How to clone ? (READ THIS OR YOU WILL FAIL)

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
