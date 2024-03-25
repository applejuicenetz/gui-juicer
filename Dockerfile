FROM debian:buster

RUN apt update && apt install -y \
    qt4-dev-tools \
    cmake \
    make \
    gcc \

RUN mkdir /work

WORKDIR /work

CMD ["tail", "-f", "/dev/null"]
