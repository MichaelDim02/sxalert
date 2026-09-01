#ifndef SXALERT_H
#define SXALERT_H

struct alert {
	int duration;
	char text_color[7];
	char bg_color[7];
	char border_color[7];
	
	int border_width;
	int text_x_padding;
	
	int min_width;
	int max_width;
	
	int x_offset;
	int y_offset;
	
	char *font;
	int text_height;

	char **lines;
	int lines_len;
};

struct xrender {
	int scr;
	Visual *visual;
	Colormap cmap;

	XftColor color;
	XftFont *font;
};

struct xwin {
	int width;
	int height;
	int x;
	int y;
	XftDraw *draw;
	Window win;	
};

struct mon {
	int monitor_count;
	XRRMonitorInfo *monitors;
};

#endif
