## This directory contains picprog & usbpwr sources

In order to make this project you must load [appropriate FT245 driver](https://www.ftdichip.com/Drivers/D2XX.htm).  We need "libftd2xx.a" static library and headers "ftd2xx.h" and "WinTypes.h" in working directory.

    $make

### Short description

**picprog** is command-line utility "picprog" which can be used for PIC10, PIC12, PIC16 programming.
**usbpwr** can be used for powering your device from USB.

### Invocation

    $picprog <processorname> <commands> <hexfilename>

Commands are:

* rp  - read program
* rd  - read data
* rc  - read configuration
* sa  - save all (only words pointed in input .hex file)
* mk  - merge input file with calibration values read previously (if selected processor needs to)
* ea  - erase all
* wp  - write program
* vp  - verify program
* wd  - write data
* vd  - verify data
* wc  - write configuration
* vc  - verify configuration


Example:

    picprog 12f683 rprcsaeawpvpwcvc smth.hex
    picprog 12f683 vcearpsarcwpvpwc smth.hex

Commands execution order is naturally determined by process logics and physics, see command list above. So both invocations above are identical - we will read first, then save, erase, write and verify.

Saved data can be found in out.hex.

### Important

1. Attach programmer to USB port.
2. Run **picprog**.
3. After prompt appeares, asking continue or not, then connect programmer to your device.
4. Confirm "y" (or maybe "n").
5. After successful (or not) programming disconnect programmer from device.
6. Answer "n".
7. Detach programmer from USB port.

The same order corresponds to **usbpwr**. Attach programmer to USB, then run **usbpwr**, and then connect programmer to your device. 

**usbpwr** commands are :

* pu  - power up
* pd  - power down
* exit  - exit

Disconnect programmer from your device and then detach from USB.




### Have a nice PIC!

