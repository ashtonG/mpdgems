mpdgems
=======

![Travis CI](https://secure.travis-ci.org/AlexBio/mpdgems.png)

**mpdgems** is a collection of some more or less useful tools for MPD (the
Music Player Daemon) intended to be used from the command line or in the background.

The collection currently includes:

 * [mpdfindby](http://alexbio.github.com/mpdgems/mpdfindby.1.html) -- find and print matching songs
 * [mpdircbot](http://alexbio.github.com/mpdgems/mpdircbot.1.html) -- control MPD playback through an IRC bot
 * [mpdislast](http://alexbio.github.com/mpdgems/mpdislast.1.html) -- stop playback after the currently playing song
 * [mpdlength](http://alexbio.github.com/mpdgems/mpdlength.1.html) -- print the total length of the current playlist
 * [mpdmmkeys](http://alexbio.github.com/mpdgems/mpdmmkeys.1.html) -- control MPD playback via the X11 multimedia keys
 * [mpdnextby](http://alexbio.github.com/mpdgems/mpdnextby.1.html) -- play next, a song that matches the given tags
 * [mpdnotify](http://alexbio.github.com/mpdgems/mpdnotify.1.html) -- stupid desktop notifier for MPD
 * [mpdplayby](http://alexbio.github.com/mpdgems/mpdplayby.1.html) -- play a song that matches the given tags

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
