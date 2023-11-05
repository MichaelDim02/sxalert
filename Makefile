sxalert:
	c99 sxalert.c -I/usr/include/freetype2/ -lXft -lX11 -lfreetype -o sxalert
install:
	mv sxalert /bin/sxalert
uninstall:
	rm /bin/sxalert
