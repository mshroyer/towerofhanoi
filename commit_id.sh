#!/bin/sh

PATH="$PATH:/usr/local/bin"

commitid=$( git rev-parse HEAD 2>/dev/null || echo "main" )
echo $commitid
