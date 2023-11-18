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
 ____________________________________________________________
| version 0.1 | language C | lines of code <200 | binaries 1 |

Simple notification system for X that works without a daemon. This is mainly for
personal use, but I'll be glad to support whoever else wants to use it.
Currently in development.

Features
========
+ Alert & exit -- no daemon
+ Freetype scalable fonts
+ UTF-8 support
+ Configurable & scriptable with shell
+ Dynamic screen positioning & sizing

Dependencies
============
To build sxalert, you'll need Xlib and Xft.

Build & install
===============
In order to build, execute the following commands:

	$ git clone https://github.com/MichaelDim02/sxalert.git && cd sxalert
	$ make sxalert

And to install on /bin:

	$ sudo make install


Usage
=====
You can simply run the tool with each argument being a line of text

	$ ./sxalert "First line" "Second line"

You can also set color, border width & duration:

	$ ./sxalert -d 1000 -b 5 -t FFFFFF -g 000000 -r FFFFFF "First line" "Second line"

These will overwrite the defaults defined in `config.h`

TODOs
=====
+ Read from stdin (perhaps as a patch)
+ Exit by event (perhaps as a patch)

Optional D-Bus daemon hack
==========================
I've created a small Shell script called sxalertd that can run as a daemon to
read the D-Bus and use sxalert to draw the notifications. The script & parsing
method are a little hacky but work quite well.
(https://github.com/MichaelDim02/sxalertd) 

Credit
======
The logo is the Alectryon! The rooster image is by Leys, Thomson W. from the
1883 book "Brett's colonists' guide and cyclopaedia" for New Zealand settlers.
The font is Byzantine, and the logo was created in GIMP.
