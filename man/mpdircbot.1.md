mpdircbot(1) -- control MPD playback through an IRC bot
=======================================================

## SYNOPSIS

`mpdircbot [OPTIONS]`

## DESCRIPTION

**mpdircbot** is a simple IRC bot that listens to private messages and controls
the MPD playback accordingly.

It recognizes the following commands:

 * `play`
 * `pause`
 * `toggle`
 * `stop`
 * `prev`
 * `next`

## OPTIONS ##

`-a, --addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server address.

`-p, --port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server port.

`-A, --irc-addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC server address (default `irc.freenode.net`).

`-P, --irc-port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC server port (default `6667`).

`-N, --nick`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC nick for the bot (default `mpdircbot`).

## AUTHOR ##

Alessandro Ghedini <al3xbio@gmail.com>

## COPYRIGHT ##

Copyright (C) 2012 Alessandro Ghedini <al3xbio@gmail.com>

This program is released under the 3 clause BSD license.