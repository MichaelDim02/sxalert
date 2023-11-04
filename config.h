/* Simple X Alert v0.1
 * Simple notification program for X
 * GNU GPL v3.0
 *
 * config.h - configuration file
 */

#define BUFFER 2054

static int duration = 4000; /* in milliseconds */

/* default appearance */

static char text_color[7]	= "e1ba9b";
static char bg_color[7]		= "081019";
static char border_color[7]	= "e1ba9b";

static int border_width	= 1;
static int text_x_padding = 10;

const char *fontname = "DejaVu Sans Mono:size=12:antialias=true";
