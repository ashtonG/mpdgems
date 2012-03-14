mpdlength(1) -- print the total length of the current playlist
==============================================================

## SYNOPSIS

`mpdlength [OPTIONS]`

## DESCRIPTION

**mpdlength** is a simple utility that prints the total length of the
current playlist or of the whole database. The output format is customizable.

## OPTIONS ##

`-f, --format`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The output format string. See [FORMAT][] for more info.

`-d, --db`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Search the MPD databse instead of the current playlist.

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

`%d` is replaced by the number of days.

`%h` is replaced by the number of hours.

`%m` is replaced by the number of minutes.

`%s` is replaced by the number of seconds.

Example: `%d days, %h hours, %m mins and %s secs`

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
