# appleJuice Qt4 GUI - juicer


### build deb package

```bash
docker run -t -i --rm -v $PWD:$PWD msoares/qt4-dev:latest bash
qmake
make
```

### build for Windows

```bash
docker run -t -i --rm -v $PWD:$PWD rouhim/nsis-installer:latest bash
CD 
makensis juicer.nsi
```
