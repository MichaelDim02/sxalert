/* sxalert - simple notification program for X
 *
 * Copyright (C) 2026 Michail Konstantinos Dimopoulos
 * License: GPL-3.0-or-later (see LICENSE)
 */

#include "sxalert.h"

/* configure values below */
static const struct alert default_alert = {
	.duration 	= 5000, /* ms */
	
	/* default appearance */
	
	.text_color	= "dfe3e3",
	.bg_color	= "1a1616",
	.border_color	= "dfe3e3",

	.border_width	= 1,
	.text_x_padding = 15,

	.min_width	= 300, 
	.max_width	= 1000,

	.x_offset	= 20, /* pixels from right */
	.y_offset	= 40, /* pixels from top */

	.font		= "DejaVu Sans Mono:size=13:antialias=true",
	.text_height 	= 10,
};
