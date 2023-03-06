my dwm config
============================

![Screenshot_2023-03-03-23-51-34_1920x1080](https://user-images.githubusercontent.com/125738354/223148206-550b9881-9cf2-49d0-9360-e3b996339eed.png)

dwm - dynamic window manager
============================
dwm is an extremely fast, small, and dynamic window manager for X.


Requirements
------------
In order to build dwm you need the Xlib header files.


Installation
------------
Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install


Running dwm
-----------
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

    DISPLAY=foo.bar:1 exec dwm

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm


Configuration
-------------
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.

Patches
-------------

[ajacenttag](https://dwm.suckless.org/patches/adjacenttag/)

[autostart](https://dwm.suckless.org/patches/autostart/)

[bar-height](https://dwm.suckless.org/patches/bar_height/)

[barpadding](https://dwm.suckless.org/patches/barpadding/)

[fibonacci](https://dwm.suckless.org/patches/fibonacci/)

[fullgaps](https://dwm.suckless.org/patches/fullgaps/)

[fullscreen](https://dwm.suckless.org/patches/fullscreen/)

[movestack](https://dwm.suckless.org/patches/movestack/)

[underlinetags](https://dwm.suckless.org/patches/underlinetags/)
