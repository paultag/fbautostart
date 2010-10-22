#!/bin/bash

RECONF=`which autoreconf`

if [ "x$RECONF" != "x" ]; then
	autoreconf -fvi
else
	echo "It looks like autoreconf is not installed."
	echo "Please install it to set up the build tool"
	echo "chain. Many thanks :)"
	echo ""
	echo " -- fbautostart ( et al )"
fi
