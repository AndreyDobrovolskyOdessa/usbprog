## This directory contains picprog & usbpwr sources

In order to make this project you must load appropriate FTDI driver from https://www.ftdichip.com/Drivers/D2XX.htm.  We need "libftd2xx.a" static library and headers "ftd2xx.h" and "WinTypes.h".

Simply $make

### Short description

This project is command-line utility "picprog" which can be used for PIC10, PIC12, PIC16 programming.
Utility "usbpwr" can be used for powering your device from USB.

### Invocation

picprog processor commands hexfilename

Commands are:

rp  - read program

rd  - read data

rc  - read configuration

sa  - save all (only words pointed in input .hex file)

ml  - merge input file with calibration values read previously

ea  - erase all

wp  - write program

vp  - verify program

wd  - write data

vd  - verify data

wc  - write configuration

vc  - verify configuration


Example: picprog 12f683 rprcsaeawpvpwcvc smth.hex

Commands execution order is natural determined by process logics and physics, see above.

Saved data can be found in out.hex.

### Important

1. Attach programmer to USB port.
2. Run picprog.
3. After prompt appeares, asking continue or not, then connect programmer to your device.
4. Confirm "y" (or maybe "n").
5. After successful (or not) programming disconnect programmer from device.
6. Answer "n".
7. Detach programmer from USB port.

The same order corresponds to usbpwr. Attach programmer to USB, then run "usbpwr", and then connect programmer to your device. 

usbpwr commands are :

pu  - power up

pd  - power down

exit  - exit

Disconnect programmer from your device and then detach from USB.




### Have a nice PIC!
