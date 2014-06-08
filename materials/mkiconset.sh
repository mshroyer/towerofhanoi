#!/bin/sh

# Script to convert an SVG image to an .icns file on OS X 10.8.  Requires
# Inkscape and OS X's iconutil program.

set -e

APPNAME=towerofhanoi
INKSCAPE=/Applications/Inkscape.app/Contents/Resources/bin/inkscape

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <svgfile>"
    exit 1
fi

svgfile="$1"

if [ -e "$APPNAME.icns" ]; then
    echo "$APPNAME.icns already exists!"
    exit 1
fi

if [ -e "$APPNAME.iconset" ]; then
    echo "$APPNAME.iconset already exists!"
    exit 1
fi

mkdir "$APPNAME.iconset"

convert_svg() {
    $INKSCAPE --export-png "$APPNAME.iconset/icon_$2.png" -w $1 -h $1 "$svgfile"
    #qlmanage -t -s $1 -o . "$svgfile"
}

convert_svg 16   16x16
convert_svg 32   16x16@2x
convert_svg 32   32x32
convert_svg 64   32x32@2x
convert_svg 128  128x128
convert_svg 256  128x128@2x
convert_svg 256  256x256
convert_svg 512  256x256@2x
convert_svg 512  512x512
convert_svg 1024 512x512@2x

iconutil -c icns "$APPNAME.iconset"
rm -r "$APPNAME.iconset"
