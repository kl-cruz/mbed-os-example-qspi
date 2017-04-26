# Getting started with qspi example application on mbed OS

This example shows how to use qspi driver in mbed-os.

## Required hardware
* A supported board - [nRF52840 DK](https://developer.mbed.org/platforms/Nordic-nRF52840-DK/).

## Import the example application

Just clone this repository.

```
mbed import https://github.com/kl-cruz/mbed-os-example-qspi
cd mbed-os-example-qspi
```

## Now compile

Invoke `mbed compile`, and specify the name of your platform and your favorite toolchain (`GCC_ARM`, `ARM`, `IAR`). For example, for the ARM Compiler 5:

```
mbed compile -m <TARGET_NAME> -t ARM
```

Your PC may take a few minutes to compile your code. At the end, you see the following result:

```
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |    65 |     4 |   38 |
| Misc                  | 33091 |  2220 |  745 |
| drivers               |  1660 |     0 |   28 |
| hal                   |   384 |     0 |   16 |
| platform              |  1205 |     4 |  264 |
| rtos                  |   137 |     4 |    4 |
| rtos/rtx              |  5915 |    20 | 7550 |
| targets/TARGET_NORDIC | 10623 |    36 | 1043 |
| Subtotals             | 53080 |  2288 | 9688 |
+-----------------------+-------+-------+------+
Allocated Heap: 234936 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 11976 bytes
Total RAM memory (data + bss + heap + stack): 248960 bytes
Total Flash memory (text + data + misc): 55368 bytes
Image: .\BUILD\NRF52840_dk\GCC_ARM\mbed-os-example-qspi.hex

```

### Program application

1. Connect your mbed device to the computer over USB.
2. Copy the application binary file to the mbed device.
3. Open terminal with proper serial port (9600 bauds, 8 bytes, one stop bit, no hw flow control).
4. Press the reset button to start the program.

## Troubleshooting

1. Make sure `mbed-cli` is working correctly and its version is `>1.0.0`.

    ```
    mbed --version
    ```

 If not, you can update it:

    ```
    pip install mbed-cli --upgrade
    ```
