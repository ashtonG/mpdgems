mpdircbot(1) -- control MPD playback through an IRC bot
=======================================================

## SYNOPSIS

`mpdircbot [OPTIONS]`

## DESCRIPTION

**mpdircbot** is a simple IRC bot that listens to private and channel
messages and controls the MPD playback accordingly.

It recognizes the following commands:

 * `play`
 * `pause`
 * `toggle`
 * `stop`
 * `prev`
 * `next`

If invited in a channel, **mpdircbot** will join it and respond to commands
sent trough the channel itself, so that everyone in the channel can control
the MPD playback. To accept commands from a channel the mpdircbot nick has to
be prepended to the command itself, e.g.:

 * `mpdircbot, play`
 * `mpdircbot, pause`
 * `mpdircbot, toggle`
 * ...

## OPTIONS ##

`-a, --addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server address.

`-p, --port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server port.

`-s, --secret`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD password.

`-A, --irc-addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC server address (default `irc.freenode.net`).

`-P, --irc-port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC server port (default `6667`).

`-N, --nick`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC nick for the bot (default `mpdircbot`).

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
