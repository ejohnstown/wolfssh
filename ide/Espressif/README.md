# wolfSSH Espressif Component Install

For more information on getting started with wolfSSL on the ESP32, see [wolfssl/IDE/Espressif/README.md](https://github.com/wolfSSL/wolfssl/blob/master/IDE/Espressif/README.md).

See also [wolfSSH - Now Available as an Espressif Managed Component Includes SSH Echo Server Example](https://www.wolfssl.com/wolfssh-now-available-as-an-espressif-managed-component-includes-ssh-echo-server-example/).

# ESP-IDF port

## Overview

ESP-IDF development framework with wolfSSL by setting *WOLFSSL_ESPIDF* definition

Including the following examples:

* SSH UART Server

 The `user_settings.h` file enables some of the hardened settings.

## Requirements
 1. [ESP-IDF development framework](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/)

 2. The wolfSSH component requires the [wolfssl component](https://github.com/wolfSSL/wolfssl/tree/master/IDE/Espressif/ESP-IDF) be installed first.


## Setup for Linux
 1. Run `setup.sh` at _/path/to_`/wolfssl/IDE/Espressif/ESP-IDF/` to deploy files into ESP-IDF tree
 2. Find Wolfssl files at _/path/to/esp_`/esp-idf/components/wolfssl/`
 3. Find [Example programs](https://github.com/wolfSSL/wolfssl/tree/master/IDE/Espressif/ESP-IDF/examples) under _/path/to/esp_`/esp-idf/examples/protocols/wolfssl_xxx` (where xxx is the project name)

## Setup for Windows ESP-IDF
 1. Run ESP-IDF Command Prompt (cmd.exe) or Run ESP-IDF PowerShell Environment. The component path should be in "%IDF_PATH%".
 2. Run `setup_win.bat` at `.\IDE\Espressif\ESP-IDF\`

```
cd ESP-IDF
setup_win.bat
```

 3. Find Wolfssl files at _/path/to/esp_`/esp-idf/components/wolfssl/`

## Setup for Windows ESP-IDF Project

Install a static copy of wolfSSH into a specific project component directory.

```
cd ESP-IDF
setup_win.bat C:\workspace\wolfssh\examples\ESP32-SSH-Server
```

## Setup for Windows VisualGDB.

Install a static copy of wolfSSH into shared VisualGDB component directory

```
cd ESP-IDF
setup_win.bat C:\SysGCC\esp32\esp-idf\v4.4
```






## Configuration
 1. The `user_settings.h` can be found in _/path/to/esp_`/esp-idf/components/wolfssl/include/user_settings.h`

## Build examples
 1. See README in each example folder

## Support
 For question please email [support@wolfssl.com]

 Note: This is tested with :
   - OS: Ubuntu 20.04.3 LTS and Microsoft Windows 10 Pro 10.0.19041 and well as WSL Ubuntu
   - ESP-IDF: ESP-IDF v4.3.2
   - Module : ESP32-WROOM-32
