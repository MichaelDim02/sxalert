/* Simple X Alert v0.1
 * Simple notification program for X
 * GNU GPL v3.0
 *
 * config.h - configuration file
 */

#define BUFFER 2054

static int duration = 5000; /* in milliseconds */

/* default appearance */

static char text_color[7]	= "dfe3e3";
static char bg_color[7]		= "1a1616";
static char border_color[7]	= "dfe3e3";

static int border_width	= 1;
static int text_x_padding = 15;

static int min_width = 300; 
static int max_width = 1000;

static int x_offset = 20; /* pixels from right */
static int y_offset = 40; /* pixels from top */

const char *fontname = "DejaVu Sans Mono:size=13:antialias=true";
static int text_height = 11;
