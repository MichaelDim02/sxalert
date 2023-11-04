# Simple X Alert 
<img src="https://mcdim.xyz/projects/sxalert/alectryon.png" alt="Alectryon rooster sxalert logo" style="height: 222px; width:200px;"/>

Simple notification system for X that works without a daemon. This is mainly for personal use, but I'll be glad to support whoever else wants to use it. Currently in development.

## Features
+ Freetype scalable fonts
+ UTF-8 support
+ Configurable & scriptable

## Dependencies
To build simple X alert, you'll need Xlib and Xft.

## Build
In order to install, execute the following commands:
```
$ git clone https://github.com/MichaelDim02/sxalert.git && cd sxalert
$ sudo make sxalert
```

## Usage
You can simply run the tool with each argument being a line of text
```
$ ./sxalert "First line" "Second line"
```
You can also set color and border width
```
$ ./sxalert -b 5 -t FFFFFF -g 000000 -r FFFFFF "First line" "Second line"
```
These will overwrite the defaults written in `config.h`

## TODOs:
+ Dynamic screen positioning
+ Width based on text
+ Exit by event (perhaps as a path)
+ Titles (perhaps as a patch)
+ (?) optional daemon for positinioning

## Credit
The rooster image is by Leys, Thomson W. from the 1883 book "Brett's colonists' guide and cyclopaedia" for New Zealand settlers. The font is Byzantine, and the logo was created in GIMP.
