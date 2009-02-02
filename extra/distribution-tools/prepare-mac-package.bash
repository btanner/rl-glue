#!/bin/bash
VERSION=`./install_root/usr/local/bin/rl_glue --pv`
DISTNAME=RL-Glue-$VERSION-and-C-Codec
PACKAGENAME=$DISTNAME.pkg
IMAGENAME=$DISTNAME.dmg

rm -Rf *.pkg
rm *.dmg
#Create the package file
/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker --root install_root --id org.rlcommunity.rlglue --version 3.0 --title "RL-Glue 3.0 and C/C++ Codec" --resources Resources --out $PACKAGENAME --root-volume-only

#Create the disk image
hdiutil create -srcfolder $PACKAGENAME $IMAGENAME