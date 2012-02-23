mpdgems
=======

**mpdgems** is a collection of some more or less useful tools for MPD (the
Music Player Daemon) intended to be used from the command line or in the background.

The collection currently includes:

 * **mpdaddall** -- add __all__ the songs to the current playlist
 * **mpdfindby** -- find and print matching songs
 * **mpdircbot** -- control MPD playback through an IRC bot
 * **mpdislast** -- stop playback after the currently playing song
 * **mpdlength** -- print the total length of the current playlist
 * **mpdmmkeys** -- control MPD playback via the X11 multimedia keys
 * **mpdnextby** -- play next, a song that matches the given tags
 * **mpdnotify** -- stupid desktop notifier for MPD
 * **mpdplayby** -- play a song that matches the given tags

Have a look at the respective manpages for more information about them.

## DEPENDENCIES

 * `libircclient` (optional, only for `mpdircbot`)
 * `libmpdclient`
 * `libnotify` (optional, only for `mpdnotify`)
 * `libx11` (optional, only for `mpdmmkeys`)

If one of the optional dependencies is not found, the relative executable will
not get built.

## BUILDING

mpdgems is distributed as source code. Install with:

~~~~
$ mkdir build && cd build
$ cmake ..
$ make
# make install
~~~~

## COPYRIGHT

Copyright (C) 2012 Alessandro Ghedini <al3xbio@gmail.com>

See COPYING for the license.
