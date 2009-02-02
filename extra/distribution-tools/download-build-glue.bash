#!/bin/bash
INSTALLDIR=`pwd`/install_root/usr/local
rm -Rf rl-glue-trunk
rm -Rf install_root
mkdir install_root
mkdir install_root/usr
mkdir install_root/usr/local
svn export http://rl-glue.googlecode.com/svn/trunk rl-glue-trunk
cd rl-glue-trunk
./configure --prefix=$INSTALLDIR
make
make install
cd ..

