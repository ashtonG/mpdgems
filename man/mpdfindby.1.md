mpdfindby(1) -- find and print matching songs
=============================================

## SYNOPSIS

`mpdfindby [OPTIONS]`

## DESCRIPTION

**mpdfindby** is a simple utility that, given some information about some songs
(i.e. title, artist and album), prints to `STDOUT` the matching entries. The
output format is configurable.

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

`-f, --format`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The output format string. `%t` is replaced by the song title, `%r` by the song
artist, `%b` by the song album, `%k` by the song number, `%d` by the song date
and `%g` by the song genre. Escaped new lines (`\n`) are also supported.
Example: `title: %t\nartist: %r\nalbum: %b\n\n`.

`-a, --addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server address.

`-p, --port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server port.

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
