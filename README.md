mpdutils
========

**mpdutils** is a collection of some more or less useful tools for MPD (the
Music Player Daemon) mostly intended to be used from the command line.

The collection currently includes:

 * **mpdaddall** -- add __all__ the songs to the current playlist
 * **mpdlen** -- print the total length of the current playlist
 * **mpdnotify** -- stupid desktop notifier for MPD

Have a look at the respective manpages for more information about them.

## DEPENDENCIES

 * `libmpdclient`
 * `libnotify` (only for `mpdnotify`)

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
