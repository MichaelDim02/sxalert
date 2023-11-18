# sxalert Makefile
# GNU GPL v3.0

CC=c99
MD=/usr/share/man/man1/

sxalert:
	$(CC) sxalert.c -I/usr/include/freetype2/ -lXft -lX11 -lfreetype -g -o sxalert
install:
	mv sxalert /bin/sxalert
	cp sxalert.1 $(MD)
uninstall:
	rm /bin/sxalert
	rm $(MD)sxalert.1
