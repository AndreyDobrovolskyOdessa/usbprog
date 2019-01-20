## This directory contains picprog sources

In order to make this project you must load "libftd2xx.a" static library from ftdichip.com, it's linked statically.

### Short description

This project is command-line utility "picprog" which can be used for PIC10, PIC12, PIC16 programming.

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

1. Connect your programmer to USB port.
2. Run picprog.
3. After prompt appeares, asking continue or not, then connect programmer to your device.
4. Confirm "y" (or maybe "n").
5. After successful (or not) programming disconnect programmer from device.
6. Answer "n".
7. Detach programmer from USB port.

### Have a nice PIC!
