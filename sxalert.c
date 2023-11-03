#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>

#define VERSION "v0.1"

#include "config.h"

static void
die(const char *msg, int code)
{
	fprintf(stderr, "%s\n", msg);
	exit(code);
}

static void
usage(const char *bin)
{
	printf("Usage: %s [arguments] \"title\" \"text\" \"text\"\n", bin);
	exit(0);
}

static void
help(void)
{
	printf("sXalert %s\n", VERSION);
	printf("Alert utility for X\n\n");
	printf("	-c color in hex:hex:hex format\n");
	printf("	-s time in seconds\n");
	printf("	-b border width in pixels\n");
	printf("	-h print help panel & exit\n\n");
}

static int
hex2int(char *str)
{
	int num;
	sscanf(str, "%x", &num);
	return num;
}

static void
draw(int border, char **lines, int linecount, int linestart)
{
	//const char *fontname = "DejaVu Sans Mono:size=12:antialias=true";

	Display *dpy = XOpenDisplay(NULL);
	if (!dpy)
		die("cannot open X11 display\n", EXIT_FAILURE);
	int scr = DefaultScreen(dpy);
	Visual *visual = DefaultVisual(dpy, scr);
	Colormap cmap = DefaultColormap(dpy, scr);

	XftColor color;
	XGlyphInfo extents;
	XftFont *font = XftFontOpenName(dpy, scr, fontname);
	if (!font)
		die("cannot load font\n", EXIT_FAILURE);
	if (!XftColorAllocName(dpy, visual, cmap, "#ffffff", &color))
	//if (!XftColorAllocName(dpy, visual, cmap, "#0dff00", &color))
		die("cannot allocate xft color\n", EXIT_FAILURE);
	XftTextExtentsUtf8(dpy, font, (XftChar8*)lines[0], strlen(lines[0]), &extents);
    	int text_height = extents.height;
    	int text_width = extents.width; /* for some reason this is always 61 */
	printf("text_width: %d\n", text_width);

	int height = (linecount - linestart) * (text_height * 2) + text_height;
	printf("height:%d\n", (linecount-linestart));

	Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 1500, 50, 400, height, border, hex2int(border_color), hex2int(bg_color));
	/* make window fixed */
	XSetWindowAttributes attributes;
	attributes.override_redirect = True;
	XChangeWindowAttributes(dpy, win, CWOverrideRedirect, &attributes);

	XSelectInput(dpy, win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, win);

	XftDraw *draw = XftDrawCreate(dpy, win, visual, cmap);

	while(1) {
		XEvent ev;

		XNextEvent(dpy, &ev);
		if (ev.type == Expose) {
			int spacing = text_height * 2;
			for (int i=linestart; i<linecount; i++) {
				XftDrawStringUtf8(draw, &color, font, 10, spacing, (XftChar8 *)lines[i], strlen(lines[i]));
				spacing += text_height * 2;
			}
		} else if (ev.type == KeyPress) {
			break;
		}
	}

	/* cleanup */
	XftColorFree(dpy, visual, cmap, &color);
	XftDrawDestroy(draw);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

int
main(int argc, char **argv)
{
	int s = 3;
	int c;
	char text[BUFFER];
	extern char *optarg;

	printf("text_color: %s\n", text_color);

	while ((c = getopt(argc, argv, "s:b:vh")) != -1 ) {
		switch (c) {
		case 'v':
			die(VERSION, EXIT_SUCCESS);
		case 'h':
			help();
			usage(argv[0]);
		case 's':
			s=atoi(optarg);
			break;
		case 'b':
			border_width=atoi(optarg); /* overwrite default in config.h */
			break;
		}
	}

	/* can be removed */
	/*printf("b=%d\n", border_width);
	if (optind < argc) {
		printf("argc=%d\n", argc);
		printf("optind=%d\n", optind);
		printf("optind=%s\n", argv[optind]);
	}*/

	draw(border_width, argv, argc, optind);

	return 0;
}
