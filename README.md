# Prerequisites
[ESP-IDF for Linux](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html)
## Connect your device
Check for USB port: 

```bash
ll /dev/ttyUSB*
```

The port displayed should be: **dev/ttyUSB0** 

In case you've setup the environment as [ESP-IDF instructions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html) but the USB port doesn't appear, you shold remove brltty.

```bash
sudo apt-get remove brltty 
```

- Reference: [askubuntu - /dev/ttyUSB0 not present in Ubuntu 22.04](https://askubuntu.com/questions/1403705/dev-ttyusb0-not-present-in-ubuntu-22-04)

# Setup your project
- Copy the sample from esp-idf root folder to your folder

```bash
cd /path/to/your/project/

cp -r ~/esp/esp-idf/examples/get-started/sample_project/. .
```

- Setup python interpreter and add ESP-IDF tools to PATH
```bash
get_idf 
```
Setting ESP32 target

```bash
idf.py set-target esp32
```
Build the Project
```bash
idf.py build
```

Flash onto the Device
```bash
idf.py -p /dev/ttyUSB0 flash
```

Monitor the Output
```bash 
idf.py -p /dev/ttyUSB0 monitor
```
Combine building, flashing, monitoring
```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

Flash Erase: to erase the entire flash memory
```bash
idf.py -p /dev/ttyUSB0 erase-flash
```








