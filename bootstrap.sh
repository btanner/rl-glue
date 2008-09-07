#! /bin/sh
libtoolize --automake -f -c
aclocal -I config
autoconf
automake --foreign --add-missing --copy
