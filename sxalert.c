/* simpleXalert v0.1
 * Simple notification program for X
 * GNU GPL v3.0
 *
 * sxalert.c - main program
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <poll.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>

#include "config.h"

#define VERSION "v0.1"

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
help(void) /* TODO: update */
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

static char *
convert_text_color_code(void)
/* adds a '#' before the color hex, as XftColorAllocName requires it */
{
	char* text_color_pnd = (char*)malloc(8);
	strncpy(text_color_pnd+1, text_color, 6);
	text_color_pnd[0] = '#';
	return text_color_pnd;
}

static void
draw(int border, char **lines, int linecount, int linestart)
{
	char text_color_pnd[8];
	strncpy(text_color_pnd, convert_text_color_code(), 8);

	Display *dpy = XOpenDisplay(NULL);
	if (!dpy)
		die("Cannot open X11 display\n", EXIT_FAILURE);
	int scr = DefaultScreen(dpy);
	Visual *visual = DefaultVisual(dpy, scr);
	Colormap cmap = DefaultColormap(dpy, scr);

	XftColor color;
	XGlyphInfo extents;
	XftFont *font = XftFontOpenName(dpy, scr, fontname);
	if (!font)
		die("Cannot load font\n", EXIT_FAILURE);
	if (!XftColorAllocName(dpy, visual, cmap, text_color_pnd, &color))
		die("Cannot allocate Xft color\n", EXIT_FAILURE);
	XftTextExtentsUtf8(dpy, font, (XftChar8*)lines[0], strlen(lines[0]), &extents);
    	int text_height = extents.height; /* TODO: int text_width = extents.width; for some reason this is always 61 */
	int height = (linecount - linestart) * (text_height * 2) + text_height;

	/* TODO: calculate position dynamically */
	Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 1500, 50, 400, height, border, hex2int(border_color), hex2int(bg_color));
	/* make window fixed */
	XSetWindowAttributes attributes;
	attributes.override_redirect = True;
	XChangeWindowAttributes(dpy, win, CWOverrideRedirect, &attributes);

	XSelectInput(dpy, win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, win);

	XftDraw *draw = XftDrawCreate(dpy, win, visual, cmap);

	while (1) {
    		struct pollfd pfd = {
        		.fd = ConnectionNumber(dpy),
        		.events = POLLIN,
    		};
		Bool pending = XPending(dpy) > 0 || poll(&pfd, 1, duration) > 0;

		if (!pending)
			break;

		XEvent ev;
		
		XNextEvent(dpy, &ev);
		
		if (ev.type == Expose) {
		    	int spacing = text_height * 2;
		    	for (int i = linestart; i < linecount; i++) {
		    		XftDrawStringUtf8(draw, &color, font, text_x_padding, spacing, (XftChar8 *)lines[i], strlen(lines[i]));
		    		spacing += text_height * 2;
			}
		} else if (ev.type == ButtonPress) { //&& ev.xbutton.button == 1) {
			printf("Event: mouse xbutton.butotn = 1\n LEFT CLICK\n");
	        	break; // Exit the loop if a key is pressed
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

	while ((c = getopt(argc, argv, "s:b:t:g:r:vh")) != -1 ) {
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
		case 't':
			strncpy(text_color, optarg, 7);
			break;
		case 'g':
			strncpy(bg_color, optarg, 7);
			break;
		case 'r':
			strncpy(border_color, optarg, 7);
			break;
		}
	}

	draw(border_width, argv, argc, optind);

	return 0;
}
