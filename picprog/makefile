picprog	:main.o picproc.o variable.o hexproc.o exchange.o
	cc -o usbprog main.o picproc.o variable.o hexproc.o exchange.o libftd2xx.a -lpthread -lrt -Wall -Wextra

main.o	:main.c variable.h exchange.h hexproc.h picproc.h
	cc -c -Wall main.c

picproc.o:picproc.c picprog.h variable.h exchange.h
	cc -c -Wall picproc.c

variable.o:variable.c picprog.h
	cc -c -Wall variable.c

hexproc.o:hexproc.c variable.h
	cc -c -Wall hexproc.c

exchange.o:exchange.c ft245pins.h
	cc -c -Wall exchange.c

usbpwr	:usbpwr.o exchange.o
	cc -o usbpwr usbpwr.o exchange.o libftd2xx.a -lpthread -lrt -Wall -Wextra
	
usbpwr.o:usbpwr.c ft245pins.h
	cc -c -Wall usbpwr.c

clean:
	rm usbprog usbpwr main.o picproc.o variable.o hexproc.o exchange.o usbpwr.o

