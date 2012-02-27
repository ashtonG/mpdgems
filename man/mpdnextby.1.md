mpdnextby(1) -- play next, a song that matches the given tags
=============================================================

## SYNOPSIS

`mpdnextby [OPTIONS]`

## DESCRIPTION

**mpdnextby** is a simple utility that, given some information about a song
(i.e. title, artist and album), plays the matching entry in the current playlist,
after the currently playing song ends.

Not all the tags are mandatory (e.g. you can only provide the title), and if
multiple matches are found, only the first one is played.

## OPTIONS ##

`-t, --title`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The song title.

`-r, --artist`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The song artist.

`-b, --album`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The song album.

`-a, --addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server address.

`-p, --port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server port.

`-s, --secret`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD password.

`-h, --help`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Show the help message.

## ENVIRONMENT ##

`MPD_HOST`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server address (overridden by `--addr`).

`MPD_PORT`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server port (overridden by `--port`).

## AUTHOR ##

Alessandro Ghedini <al3xbio@gmail.com>

## COPYRIGHT ##

Copyright (C) 2012 Alessandro Ghedini <al3xbio@gmail.com>

This program is released under the 3 clause BSD license.
