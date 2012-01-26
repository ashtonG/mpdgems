mpdutils
========

![status](http://stillmaintained.com/AlexBio/mpdutils.png)

**mpdutils** is a collection of some more or less useful MPD tools mostly
intended to be used from the command line.

The collection currently includes:

 * mpdaddall(1) -- add __all__ the songs to the current playlist
 * mpdlen(1) -- print the total length of the current playlist
 * mpdnotify(1) -- stupid desktop notifier for MPD

Have a look at the respective manpages for more infor about them.

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
