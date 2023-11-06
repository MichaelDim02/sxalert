/* Simple X Alert v0.1
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
help(char *bin)
{
	printf("sxalert %s - Alert utility for X\n\n", VERSION);
	printf("	-t text hex color (eg FFFFFF)\n");
	printf("	-g background hex color\n");
	printf("	-r border hex color\n");
	printf("	-b border width in pixels\n");
	printf("	-d duration in milliseconds\n");
	printf("	-v print version & exit\n\n");
	printf("	-h print help panel & exit\n\n");
	printf("Usage: %s [arguments] \"text\" \"text\" \"text\"\n", bin);
	exit(0);
}

static int
hex2int(char *str)
{
	int num;
	sscanf(str, "%x", &num);
	return num;
}

static char *
convert_text_color_code(void) /* adds a '#' before the color hex, as XftColorAllocName requires it */
{
	char* text_color_pnd = (char*)malloc(8);
	strncpy(text_color_pnd+1, text_color, 6);
	text_color_pnd[0] = '#';
	return text_color_pnd;
}

int
get_max(int arr[], int len)
{
	int max = -1;
	for (int i=0; i<len; i++) {
		if (arr[i] > max)
			max = arr[i];
	}

	return max;
}

static int
get_width(Display *dpy, XftFont *font, char **lines, int length)
{
	XGlyphInfo ext;
	int width = 0;
	if (length > 0) {
		int width_lines[length];
		int j = 0;
		for (int i = 0; i < length; i++) {
			XftTextExtentsUtf8(dpy, font, (XftChar8*)lines[i], strlen(lines[i]), &ext);
    			width_lines[i] = ext.xOff;
			j++;
		}

		int text_width = get_max(width_lines, length);
		width = (text_x_padding * 2) + text_width;
	}

	if (width < min_width)
		return min_width;
	else if (width > max_width)
		return max_width;
	else
		return width;
}

static void
write_text(Display *dpy, XftDraw *draw, XftColor color, XftFont *font, int text_height, char **lines, int length)
{
	while (1) {
    		struct pollfd pfd = {
        		.fd = ConnectionNumber(dpy),
        		.events = POLLIN,
    		};
		int pending = XPending(dpy) > 0 || poll(&pfd, 1, duration) > 0;

		if (!pending)
			break;

		XEvent ev;
		
		XNextEvent(dpy, &ev);
		
		if (ev.type == Expose) {
		    	int spacing = text_height * 2;
		    	for (int i = 0; i < length; i++) {
				if (length != 0)
		    			XftDrawStringUtf8(draw, &color, font, text_x_padding, spacing, (XftChar8 *)lines[i], strlen(lines[i]));
		    		spacing += text_height * 2;
			}
		}
	}
}

static void
draw(int border, int duration, char **lines, int length)
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
	
	int width = get_width(dpy, font, lines, length);
	int height = length * (text_height * 2) + text_height;

	int count_screens = ScreenCount(dpy);
        Screen *screen = ScreenOfDisplay(dpy, 0);
	int x = screen->width - width - x_offset;

	Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), x, y_offset, width, height, border, hex2int(border_color), hex2int(bg_color));
	XSetWindowAttributes attributes;
	attributes.override_redirect = True;
	XChangeWindowAttributes(dpy, win, CWOverrideRedirect, &attributes);
	XSelectInput(dpy, win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, win);

	XftDraw *draw = XftDrawCreate(dpy, win, visual, cmap);

	write_text(dpy, draw, color, font, text_height, lines, length);

	XftColorFree(dpy, visual, cmap, &color);
	XftDrawDestroy(draw);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

int
main(int argc, char **argv)
{
	int c, s = 3;
	char text[BUFFER];
	extern char *optarg;

	while ((c = getopt(argc, argv, "d:b:t:g:r:vh")) != -1 ) {
		switch (c) {
		case 'v':
			die(VERSION, EXIT_SUCCESS);
		case 'h':
			help(argv[0]);
		case 'd':
			duration=atoi(optarg);
			break;
		case 'b':
			border_width=atoi(optarg);
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

	int lines_len=argc-optind;
	char** lines = argv + optind; /* get lines to print */
	draw(border_width, duration, lines, lines_len);

	return 0;
}
