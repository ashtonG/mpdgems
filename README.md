mpdutils
========

**mpdutils** is a collection of some more or less useful tools for MPD (the
Music Player Daemon) intended to be used from the command line or in the background.

The collection currently includes:

 * **mpdaddall** -- add __all__ the songs to the current playlist
 * **mpdlen** -- print the total length of the current playlist
 * **mpdmmkeys** -- control MPD playback via the X11 multimedia keys
 * **mpdnotify** -- stupid desktop notifier for MPD
 * **mpdplayby** -- play a song that matches the given tags

Have a look at the respective manpages for more information about them.

## DEPENDENCIES

 * `libmpdclient`
 * `libnotify` (only for `mpdnotify`)
 * `xlib` (only for `mpdmmkeys`)

## BUILDING

mpdutils is distributed as source code. Install with:

~~~~
$ git clone git://github.com/AlexBio/mpdutils.git && cd mpdutils
$ cmake .
$ make
# make install
~~~~

## COPYRIGHT

Copyright (C) 2012 Alessandro Ghedini <al3xbio@gmail.com>

See COPYING for the license.
