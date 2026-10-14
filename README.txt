                               ~-.
             ,,,;            ~-.~-.~-
            (.../           ~-.~-.~-.~-.~-.
           <} o~`,         ~-.~-.~-.~-.~-.~-.
            (/    \      ~-.~-.~-.~-.~-.~-.~-.
             ;    \    ~-.~-.~-.~-.~-.~-.~-.
            ;     {_.~-.~-.~-.~-.~-.~-.~
           ;:  .-~`    ~-.~-.~-.~-.~-.
          ;.: :'    ._   ~-.~-.~-.~-.~-
           ;::`-.    '-._  ~-.~-.~-.~-
            ;::. `-.    '-,~-.~-.~-.
             ';::::.`''-.-'
               ';::;;:,:'
                  '||"
                  / |
                ~` ~"'mic

                       '||                    .   
 ....  ... ...  ....    ||    ....  ... ..  .||.  
||. '   '|..'  '' .||   ||  .|...||  ||' ''  ||   
. '|..   .|.   .|' ||   ||  ||       ||      ||   
|'..|' .|  ||. '|..'|' .||.  '|...' .||.     '|.' 
      

Simple X Alert 
==============

Simple alert utility for X.

Features
--------
+ Alert & exit -- no daemon
+ Freetype scalable fonts
+ UTF-8 support
+ Configurable & scriptable with shell
+ Dynamic screen positioning & sizing
+ Runtime color customization
+ Multi-monitor support
+ Concurrent notifications serialization

Dependencies
------------
A c99 compiler, make, libc, Xlib and Xft.

Build & install
---------------
To build:

	$ git clone git://mcdim.xyz/sxalert.git && cd sxalert
	$ make sxalert

To install:

	$ sudo make install

Usage
-----
You can simply run the tool with each argument representing a line of text:

	$ sxalert "First line" "Second line"

You can also set color, border width & duration:

	$ sxalert -d 1000 -b 5 -t FFFFFF -g 000000 -r FFFFFF "Hello world!"

These will override the defaults defined in `config.h`

sxalert can read from standard input:

	$ sxalert -s < banner.txt

	$ { printf '%s\n' '===== BUILD FAILED ====='; \
	    tail -n 20 build.log; } \
	  | ./sxalert -s

Return values
-------------
sxalert returns:
0  - Left click on the notification window
1  - Middle click on the notification window
2  - Right click on the notification window
3  - Time out

Yes, this does mean that sxalert returns non-zero values even with normal
execution, but for a good reason.  This makes sxalert excellent for shell
scripts. For example:

You can set your mail to read with an acknowledgment:

	$ sxalert "New Mail" && mail-set-to-read

Or you can create a more elaborate interaction:

	sxalert "New Mail: X" "Left click to set to read" \
		"Middle click to delete" \
		"Right click to close"
	rc=$?

	case $rc in
	0)
		mail-set-to-read x
		;;
	1)
		mail-delete x
		;;
	2)
		echo "Unread mail"
		;;
	esac

Optional D-Bus daemon hack
--------------------------
I've created a small Shell script called sxalertd that can run as a pseudo
daemon that intercepts D-Bus and uses sxalert to draw the notifications.
A little hacky but works well enough.
(https://mcdim.xyz/git/sxalertd/log.html) 

Colophon
--------
Author: Michail Konstantinos Dimopoulos
Copyright (C) 2023, 2026 Michail Konstantinos Dimopoulos
License: GNU General Public License 3.0 or later (GPL-3.0-or-later)
         See LICENSE for more information
Patches: <hackers@mcdim.xyz>

Credit
------
The logo is the Alectryon! The rooster image is by Leys, Thomson W. from
the 1883 book "Brett's colonists' guide and cyclopaedia" for New Zealand
settlers. The font is Byzantine, and the logo was created in GIMP.
