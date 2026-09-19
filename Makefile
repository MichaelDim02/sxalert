# sxalert Makefile
# GNU GPL v3.0

CC=c99
MD=/usr/share/man/man1/

sxalert: sxalert.c sxalert.h config.h
	$(CC) sxalert.c -I/usr/include/freetype2/ -lXft -lX11 \
	-lfreetype -lXrandr -g -o sxalert \
	-Wall -Wextra -Wpedantic \
	-Wconversion -Wsign-conversion \
	-Wshadow -Wstrict-prototypes \
	-Wmissing-prototypes -Wformat=2
install:
	mv sxalert /bin/sxalert
	cp sxalert.1 $(MD)
uninstall:
	rm /bin/sxalert
	rm $(MD)sxalert.1
clean:
	rm sxalert
