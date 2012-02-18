mpdlength(1) -- print the total length of the current playlist
==============================================================

## SYNOPSIS

`mpdlength [OPTIONS]`

## DESCRIPTION

**mpdlength** is a simple utility that prints to `STDOUT` the total length of
the current playlist. The output format is configurable.

## OPTIONS ##

`-f, --format`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The output format string. `%d` is replaced by the number of days, `%h` by the
number of hours, `%m` by the number of minutes and `%s` by the number of seconds.
Example: `%d days, %h hours, %m mins and %s secs`

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
