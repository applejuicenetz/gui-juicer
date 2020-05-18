#!/usr/bin/env bash

set -e

release="1"
version=$(grep VERSION ../src/src.pro | cut -d\- -f2)
osc_dir=../build/
qt_filename=juicer-$version.tar.gz
qt_filename_un=juicer-$version.tar
filename=juicer-$version-$release.tar.gz
filepath=$osc_dir/$filename

mkdir -p ../build

cd ../src
qmake
make
make dist
# removing moc files from source archive
gunzip $qt_filename
tar --delete -f $qt_filename_un $(tar --list -f $qt_filename_un | grep moc_)
gzip $qt_filename_un
cd -

mv ../src/$qt_filename $filepath
echo using $filename

md5sum=$(md5sum $filepath | cut -d\  -f1)
size=$(stat -c%s $filepath)
date=$(date -R)

sed "s/\$version/$version/g" juicer.spec-template | sed "s/\$release/$release/g" | sed "s/\$filename/$filename/g" >$osc_dir/juicer.spec
sed "s/\$version/$version/g" juicer.dsc-template | sed "s/\$release/$release/g" | sed "s/\$filename/$filename/g" >$osc_dir/juicer.dsc
sed "s/\$version/$version/g" debian.changelog-template | sed "s/\$release/$release/g" | sed "s/\$filename/$filename/g" >$osc_dir/debian.changelog

sed -i "s/\$md5sum/$md5sum/g" $osc_dir/juicer.dsc
sed -i "s/\$size/$size/g" $osc_dir/juicer.dsc

sed -i "s/\$date/$date/g" $osc_dir/debian.changelog
sed -i "s/\$message/$1/g" $osc_dir/debian.changelog
