FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    software-properties-common ca-certificates build-essential git \
 && rm -rf /var/lib/apt/lists/*

# Qt4 aus PPA
#RUN add-apt-repository -y ppa:rock-core/qt4 \
# && apt-get update && apt-get install -y --no-install-recommends \
#    qt4-default qt4-qmake libqt4-dev \
# && rm -rf /var/lib/apt/lists/*

WORKDIR /work

# qmake + make
# RUN qmake juicer.pro && make -j"$(nproc)"

# Artefakte landen in /work/src oder /work (je nach .pro)