# bobbycar-boardcomputer-firmware

[![Build Status](https://travis-ci.org/bobbycar-graz/bobbycar-boardcomputer-firmware.svg?branch=master)](https://travis-ci.org/bobbycar-graz/bobbycar-boardcomputer-firmware)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/04c264db40c44276833f43b9c083dc14)](https://app.codacy.com/gh/bobbycar-graz/bobbycar-boardcomputer-firmware?utm_source=github.com&utm_medium=referral&utm_content=bobbycar-graz/bobbycar-boardcomputer-firmware&utm_campaign=Badge_Grade_Dashboard)

## How to clone and compile?

```
git clone --recursive git@github.com:bobbycar-graz/bobbycar-boardcomputer-firmware.git
cd bobbycar-boardcomputer-firmware/
./esp-idf/install.sh
. esp-idf/export.sh
idf.py -p /dev/ttyUSB0 -b 921600 flash monitor
```

## How to wire all connections?

![Wiring diagram](/img/wiring.png)

### -DFEATURE_DPAD
![dpad switches with 5 wires](/img/dpadsw.png)

### -DFEATURE_DPAD_3WIRESW
![3 wire switches](/img/3wiresw.png)

### -DFEATURE_ROTARY
![rotary encoder](/img/rotary.png)
