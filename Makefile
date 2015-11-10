DIR_PREFIX = /usr/avr/

all:
	cd src; make

install:
	sudo cp -f -R include lib $(DIR_PREFIX)

uninstall:
	sudo rm -r $(DIR_PREFIX)/include/avrutil
	sudo rm $(DIR_PREFIX)/lib/libavrutil.a

clean:
	cd src; make clean
	rm -r lib
