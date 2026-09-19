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

Simple alert pop-up utility for X.

Features
--------
+ Alert & exit -- no daemon
+ Freetype scalable fonts
+ UTF-8 support
+ Configurable & scriptable with shell
+ Dynamic screen positioning & sizing

Dependencies
------------
A c99 compiler, make, Xlib and Xft.

Build & install
---------------
In order to build, execute the following commands:

	$ git clone git://mcdim.xyz/sxalert.git && cd sxalert
	$ make sxalert

And to install on /bin:

	$ sudo make install

Usage
-----
You can simply run the tool with each argument being a line of text

	$ sxalert "First line" "Second line"

You can also set color, border width & duration:

	$ sxalert -d 1000 -b 5 -t FFFFFF -g 000000 -r FFFFFF "Hello world!"

These will overwrite the defaults defined in `config.h`

Return values
-------------
sxalert returns:
0  - Left click on the notification window
1  - Midde click on the notification window
2  - Right click on the notification window
3  - Time out

Yes, this does mean that sxalert returns non-zero values even with normal
execution, but for a good reason.  This makes sxalert excellent for shell
scripts. For example:

You can set your mail to read with an acknowledgment

	$ sxalert "New Mail" && mail-set-to-read

Or you can create a more elaborate interaction

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

Optional D-Bus daemon hack
--------------------------
I've created a small Shell script called sxalertd that can run as a pseudo
daemon that intercepts D-Bus and uses sxalert to draw the notifications.
It's not a proper daemon, and does not own the process, but can work fine.
method are a little hacky but work quite well.
(https://github.com/MichaelDim02/sxalertd) 

Credit
------
The logo is the Alectryon! The rooster image is by Leys, Thomson W. from
the 1883 book "Brett's colonists' guide and cyclopaedia" for New Zealand
settlers. The font is Byzantine, and the logo was created in GIMP.
