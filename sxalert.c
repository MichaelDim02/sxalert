/* Simple X Alert v0.1
 * Simple notification program for X
 * GNU GPL v3.0+
 *
 * sxalert.c - main program
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>
#include <poll.h>
#include <X11/Xft/Xft.h>

#include "config.h"

#define VERSION "v0.1"

static inline void
die(const char *msg, int code)
{
	fprintf(stderr, "%s\n", msg);
	exit(code);
}

static inline void
help(char *bin)
{
	printf("sxalert %s - Alert utility for X\n\n"
	       "	-t text hex color (eg FFFFFF)\n"
	       "	-g background hex color\n"
	       "	-r border hex color\n"
	       "	-b border width in pixels\n"
	       "	-d duration in milliseconds\n\n"
	       "	-v print version & exit\n"
	       "	-h print help panel & exit\n\n"
	       "Usage: %s [arguments] \"text\" \"text\" \"text\"\n",
	       VERSION, bin);
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
convert_text_color_code(struct alert *a) /* adds a '#' before the color hex, */
{                             /* as XftColorAllocName requires it */
	char* text_color_pnd = (char*)malloc(8);
	strncpy(text_color_pnd+1, a->text_color, 6);
	text_color_pnd[0] = '#';
	text_color_pnd[7] = '\0';
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
get_width(Display *dpy, XftFont *font, struct alert *a)
{
	XGlyphInfo ext;
	int width = 0;
	if (a->lines_len > 0) {
		int width_lines[a->lines_len];
		int j = 0;
		for (int i = 0; i < a->lines_len; i++) {
			XftTextExtentsUtf8(dpy, font, (XftChar8*)a->lines[i], strlen(a->lines[i]), &ext);
    			width_lines[i] = ext.xOff;
			j++;
		}

		int text_width = get_max(width_lines, a->lines_len);
		width = (a->text_x_padding * 2) + text_width;
	}

	if (width < a->min_width)
		return a->min_width;
	else if (width > a->max_width)
		return a->max_width;
	return width;
}

static void
write_text(Display *dpy, Window w, XftDraw *draw, XftColor color,
	   XftFont *font, struct alert *a)
{
	while (1) {
    		struct pollfd pfd = {
        		.fd = ConnectionNumber(dpy),
        		.events = POLLIN,
    		};
		int pending = XPending(dpy) > 0 || poll(&pfd, 1, a->duration) > 0;

		if (!pending)
			break;

		XEvent ev;
		XSelectInput(dpy, w, ExposureMask | ButtonPressMask);
		XNextEvent(dpy, &ev);

		if (ev.type == Expose) {
		    	int spacing = a->text_height * 2;
		    	for (int i = 0; i < a->lines_len; i++) {
				if (a->lines_len != 0)
		    			XftDrawStringUtf8(draw, &color, font, a->text_x_padding, spacing, (XftChar8 *)a->lines[i], strlen(a->lines[i]));
		    		spacing += a->text_height * 2;
			}
		} else if (ev.type == ButtonPress) {
			return;
		}
	}
}

static void
init_render(Display *dpy, struct xrender *r, struct alert *a)
{
	char text_color_pnd[8];
	strncpy(text_color_pnd, convert_text_color_code(a), 8);

	r->scr = DefaultScreen(dpy);
	r->visual = DefaultVisual(dpy, r->scr);
	r->cmap = DefaultColormap(dpy, r->scr);

	r->font = XftFontOpenName(dpy, r->scr, a->font);
	if (!r->font)
		die("Cannot load font\n", EXIT_FAILURE);
	if (!XftColorAllocName(dpy, r->visual, r->cmap, text_color_pnd, &(r->color)))
		die("Cannot allocate Xft color\n", EXIT_FAILURE);
}

static inline Display *
get_display(void)
{
	Display *dpy = XOpenDisplay(NULL);
	if (!dpy)
		die("Cannot open X11 display\n", EXIT_FAILURE);
	return dpy;
}

static void
init_window(Display *dpy, struct xrender *r, struct xwin *w, struct alert *a)
{
	w->width = get_width(dpy, r->font, a);
	w->height = a->lines_len * (a->text_height * 2) + a->text_height;

	//int count_screens = ScreenCount(dpy);
        Screen *screen = ScreenOfDisplay(dpy, 0);
	w->x = screen->width - w->width - a->x_offset;

	w->win = XCreateSimpleWindow(dpy, RootWindow(dpy, r->scr), w->x,
					 a->y_offset, w->width, w->height, a->border_width,
					 hex2int(a->border_color),
					 hex2int(a->bg_color));
	XSetWindowAttributes attributes = {
        	.override_redirect = True
	};
	XChangeWindowAttributes(dpy, w->win, CWOverrideRedirect, &attributes);
	XSelectInput(dpy, w->win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, w->win);

	w->draw = XftDrawCreate(dpy, w->win, r->visual, r->cmap);
}

static void
cleanup(Display *dpy, struct xrender *r, struct xwin *w, struct alert *a)
{
	XftColorFree(dpy, r->visual, r->cmap, &r->color);
	XftDrawDestroy(w->draw);
	XDestroyWindow(dpy, w->win);
	XCloseDisplay(dpy);
}

static void
draw(struct alert a)
{
	Display *dpy = get_display();

	struct xrender r;
	init_render(dpy, &r, &a);

	struct xwin w;
	init_window(dpy, &r, &w, &a);

	write_text(dpy, w.win, w.draw, r.color, r.font, &a);

	cleanup(dpy, &r, &w, &a);
}

int
main(int argc, char **argv)
{
	int c = 3;
	extern char *optarg;

	struct alert a = default_alert;

	while ((c = getopt(argc, argv, "d:b:t:g:r:vh")) != -1 ) {
		switch (c) {
		case 'v':
			die(VERSION, EXIT_SUCCESS);
		case 'h':
			help(argv[0]);
		case 'd':
			a.duration=atoi(optarg);
			break;
		case 'b':
			a.border_width=atoi(optarg);
			break;
		case 't':
			strncpy(a.text_color, optarg, 7);
			break;
		case 'g':
			strncpy(a.bg_color, optarg, 7);
			break;
		case 'r':
			strncpy(a.border_color, optarg, 7);
			break;
		}
	}

	a.lines_len = argc - optind;
	a.lines = argv + optind; /* get lines to print */
	draw(a);

	return 0;
}
