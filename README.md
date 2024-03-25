# appleJuice Qt4 GUI - juicer


## create build container

```bash
docker build -f Dockerfile -t applejuicenet/gui-juicer:dev .
```

### build debian package

```bash
docker run -t -i --rm -v $PWD:/work applejuicenet/gui-juicer:dev bash
cd packaging
./create_release.sh
```

### build for Windows

```bash
# TODO 
```

### build on macOS

```bash
brew tap cartr/qt4
brew install qt@4
cd src
export QMAKESPEC=macx-g++
/usr/local/Cellar/qt@4/4.8.7_6/bin/qmake
# make clean
make
```

## nice2know

Update kamen über

`applejuicer.net/update/update.xml`
