# usbprog
USB serial programmer. Power (out &amp; in), logical out, bidirectional line, 0-Vdd-Vpgm out.

Based on FT245RL utilizing its synchronous bit-bang mode.
Power out (VDDOUT 5V from USB) with limited rise speed of >0.05 V/ms.This allows to power devices with high capacity - up to 4700 uF, in case FT245RL is programmed to source 500 mA (see ftdichip.com for further information).

VDDIN for devices operated less than 5V. Assumes VDDOUT -> device on-board lowering stabilizer -> VDDIN.

VDD discharge circuit with limited fall speed.

Programming voltage got using MC34063 boost convertor. Programming voltage set manually.

Can be used for programming externelly powered devices.
