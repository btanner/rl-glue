#!/bin/bash
INSTALLDIR=`pwd`/install_root/usr/local
rm -Rf codec-trunk
svn export http://rl-glue-ext.googlecode.com/svn/trunk/projects/codecs/C/ codec-trunk
cd codec-trunk
./configure --prefix=$INSTALLDIR --with-rl-glue=$INSTALLDIR
make
make install
cd ..

