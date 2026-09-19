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
#include <X11/extensions/Xrandr.h>

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
			XftTextExtentsUtf8(dpy, font,
				           (XftChar8*)a->lines[i],
					   strlen(a->lines[i]),
					   &ext);
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

static int
find_window(struct xwin *windows, int count, Window id)
{
	for (int i = 0; i < count; ++i) {
		if (windows[i].win == id)
			return i;
	}

	return -1;
}

static void
write_text(Display *dpy, struct xwin *windows, int count,
           XftColor color, XftFont *font, struct alert *a)
{
	while (1) {
		struct pollfd pfd = {
		        .fd = ConnectionNumber(dpy),
		        .events = POLLIN,
		};
		
		int pending = XPending(dpy) > 0 ||
			      poll(&pfd, 1, a->duration) > 0;

		if (!pending)
		        break;
		
		XEvent ev;
		XNextEvent(dpy, &ev);
	
		int i = find_window(windows, count, ev.xany.window);
		
		if (i < 0)
		        continue;
		
		if (ev.type == Expose) {
			int spacing = a->text_height * 2;
			
			for (int line = 0; line < a->lines_len; ++line) {
				XftDrawStringUtf8(windows[i].draw, &color,
					font, a->text_x_padding, spacing,
				        (XftChar8 *)a->lines[line],
				        (int)strlen(a->lines[line])
				);
				
				spacing += a->text_height * 2;
			}
		} else if (ev.type == ButtonPress) {
			switch (ev.xbutton.button) {
			case Button1: /* left click */
				exit(0);
				break;
			case Button2: /* middle click */
				exit(1);
				break;
			case Button3: /* right click */
				exit(2);
				break;
			}
		}
	}
}


static void
init_render(Display *dpy, struct xrender *r, struct alert *a)
{
	char text_color_pnd[8];
	snprintf(text_color_pnd, 8, "#%s", a->text_color);

	r->scr = DefaultScreen(dpy);
	r->visual = DefaultVisual(dpy, r->scr);
	r->cmap = DefaultColormap(dpy, r->scr);

	r->font = XftFontOpenName(dpy, r->scr, a->font);
	if (!r->font)
		die("Cannot load font\n", -1);
	if (!XftColorAllocName(dpy, r->visual, r->cmap,
			       text_color_pnd, &(r->color)))
		die("Cannot allocate Xft color\n", -1);
}

static void
init_window(Display *dpy, struct xrender *r, struct xwin *w,
	    struct alert *a, XRRMonitorInfo * monitor)
{
	w->width = get_width(dpy, r->font, a);
	w->height = a->lines_len * (a->text_height * 2)
		    + a->text_height;

        Screen *screen = ScreenOfDisplay(dpy, 0);
	w->x = monitor->x + monitor->width - w->width - a->x_offset;
	w->y = monitor->y + a->y_offset;


	w->win = XCreateSimpleWindow(dpy, RootWindow(dpy, r->scr), w->x,
					 a->y_offset, w->width, w->height,
					 a->border_width,
					 hex2int(a->border_color),
					 hex2int(a->bg_color));
	XSetWindowAttributes attr = {
        	.override_redirect = True
	};
	XChangeWindowAttributes(dpy, w->win, CWOverrideRedirect, &attr);
	XSelectInput(dpy, w->win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, w->win);

	w->draw = XftDrawCreate(dpy, w->win, r->visual, r->cmap);

	XSelectInput(dpy, w->win, ExposureMask | ButtonPressMask);
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
	Display *dpy = XOpenDisplay(NULL);
	if (!dpy)
		die("Cannot open X11 display\n", -1);

	struct xrender r;
	init_render(dpy, &r, &a);

	struct mon m;
	m.monitor_count = 1;
	m.monitors = XRRGetMonitors(dpy, RootWindow(dpy, r.scr),
				    True, &m.monitor_count);
	
	/* for debugging 
	printf("Found %d monitors\n", m.monitor_count);
	for (int i = 0; i < m.monitor_count; i++) {
		printf("Monitor %d: x=%d y=%d width=%d height=%d "
		      "primary=%d\n", i, m.monitors[i].x, m.monitors[i].y,
		      m.monitors[i].width, m.monitors[i].height,
		      m.monitors[i].primary);
	} */
	
	struct xwin *w = calloc(m.monitor_count, sizeof(struct xwin));
	if (!w)
		die("Cannot allocate window array\n", -1);

	for (int i = 0; i < m.monitor_count; i++) {
		init_window(dpy, &r, &w[i], &a, &m.monitors[i]);

	}

	write_text(dpy, w, m.monitor_count, r.color, r.font, &a);

	//cleanup(dpy, &r, &w, &a); /* TODO */
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
			if (a.duration == 0) {
				a.duration = -1;
			}
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

	return 4;
}
