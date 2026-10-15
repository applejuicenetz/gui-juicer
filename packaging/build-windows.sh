#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
docker build --platform linux/amd64 -f "$root/Dockerfile.windows" -t juicer-build:windows "$root"
mkdir -p "$root/dist"
docker run --rm --platform linux/amd64 -v "$root:/source:ro" -v "$root/dist:/output" juicer-build:windows sh -ec '
cp -a /source/src /build/src
cp /source/juicer.pro /build/
cd /build
find . -name Makefile -delete
find . -name "*.o" -delete
find . -name "moc_*.cpp" -delete
find . -name "qrc_*.cpp" -delete
find . -name "ui_*.h" -delete
x86_64-w64-mingw32.static-qmake-qt4 juicer.pro
make -j4
file src/release/juicer.exe
cp src/release/juicer.exe /output/juicer-1.3-windows-x86_64.exe
'
docker run --rm -v "$root:/source:ro" -v "$root/dist:/output" ubuntu:22.04 sh -ec '
apt-get update -qq
apt-get install -y -qq --no-install-recommends nsis >/dev/null
makensis -DSOURCE=/source -DOUTPUT=/output /source/packaging/juicer.nsi
'
sha256sum "$root/dist/juicer-1.3-windows-x86_64"*.exe
