# sxalert Makefile
# GNU GPL v3.0+

CC=c99
VERSION=1.0
DIST=sxalert-$(VERSION)

SRC = sxalert.c sxalert.h config.h
DOC = sxalert.1 README.txt LICENSE
FILES = $(SRC) $(DOC) Makefile

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
MANDIR ?= $(PREFIX)/share/man/man1
DESTDIR ?=


CFLAGS = -Wall -Wextra -Wpedantic \
         -Wconversion -Wsign-conversion \
         -Wshadow -Wstrict-prototypes \
         -Wmissing-prototypes -Wformat=2
PKGS = xft xrandr x11
CPPFLAGS += $(shell pkg-config --cflags $(PKGS))
LDLIBS += $(shell pkg-config --libs $(PKGS))

.PHONY: all clean install uninstall dist

all: sxalert

sxalert: $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) sxalert.c -o $@ $(LDLIBS)

install:
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)
	install -m 755 sxalert   $(DESTDIR)$(BINDIR)
	install -m 644 sxalert.1 $(DESTDIR)$(MANDIR)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/sxalert
	rm -f $(DESTDIR)$(MANDIR)/sxalert.1

clean:
	rm -f sxalert

dist:
	mkdir $(DIST)
	cp $(FILES) $(DIST)/
	tar -czf $(DIST).tar.gz $(DIST)
	rm -rf $(DIST)
	sha256sum $(DIST).tar.gz > $(DIST).tar.gz.sha256
