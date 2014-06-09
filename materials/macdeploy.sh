#!/bin/sh

# Mac OS X deployment script

set -e

if [ $# -ne 1 ]; then
    echo "Usage: macdeploy.sh <towerofhanoi.app>"
    exit 1
fi

APP_BUNDLE="$1"
PROJECT_FILE="$(dirname $0)/../towerofhanoi.pro"
VERSION=$(awk '{ if ($1 == "ABOUT_VERSION") print $3; }' <"$PROJECT_FILE")
DMG_FILE="towerofhanoi-mac-$VERSION.dmg"

if [ -e "towerofhanoi" ]; then
    echo "towerofhanoi folder already exists!"
    exit 1
fi

if [ -e "$DMG_FILE" ]; then
    echo "$DMG_FILE already exists!"
    exit 1
fi

mkdir towerofhanoi
cp -pr "$APP_BUNDLE" towerofhanoi/towerofhanoi.app
macdeployqt towerofhanoi/towerofhanoi.app
mv towerofhanoi/towerofhanoi.app "towerofhanoi/Tower of Hanoi.app"
cp "$(dirname $0)/../COPYING.txt" towerofhanoi/
cp "$(dirname $0)/../README.txt" towerofhanoi/

hdiutil create -volname "Tower of Hanoi $VERSION" \
    -srcfolder towerofhanoi -ov -format UDBZ "$DMG_FILE"

rm -rf towerofhanoi
