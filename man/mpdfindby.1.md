mpdfindby(1) -- find and print matching songs
=============================================

## SYNOPSIS

`mpdfindby [OPTIONS]`

## DESCRIPTION

**mpdfindby** is a simple utility that searches for and prints the songs
that match the given paramenters (e.g. artist, album, ...). It can search
in the current playlist or in the whole database, and the output format can
be customized. Optionally a new playlist can be created out of the matching
songs.

## OPTIONS ##

`-A, --all`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Match all the songs.

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
The output format string.  See [FORMAT][] for more info.

`-d, --db`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Search the MPD databse instead of the current playlist.

`-l, --list`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Create a playlist with the matching songs.

`-k, --keep`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Do not delete the current playlist (to be used with -l).

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

## FORMAT ##

The interpreted sequences in the format string are:

`%t` is replaced by the song title.

`%r` is replaced by the song artist.

`%b` is replaced by the song album.

`%k` is replaced by the song number.

`%d` is replaced by the song date

`%g` is replaced by the song genre.

Escaped new lines (`\n`) are also supported.

Example: `title: %t\nartist: %r\nalbum: %b\n\n`.

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
