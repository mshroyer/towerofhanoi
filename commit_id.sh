#!/bin/sh

hg --debug id -i | awk '{ print substr($0,0,40); }' 2>/dev/null || echo "default"
