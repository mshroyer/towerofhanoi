#!/bin/sh

PATH="$PATH:/usr/local/bin"

commitid=$( hg --debug id -i 2>/dev/null || echo "default" )
echo $commitid | awk '{ print substr($0,0,40); }'

